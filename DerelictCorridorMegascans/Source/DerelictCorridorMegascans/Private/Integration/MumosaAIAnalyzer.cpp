#include "Integration/MumosaAIAnalyzer.h"
#include "Engine/GameViewportClient.h"
#include "Slate/SceneViewport.h"
#include "ImageUtils.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Misc/Base64.h"
#include "Serialization/BufferArchive.h"
#include "RenderingThread.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "EngineUtils.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogMumosaAI, Log, All);

void UMumosaAIAnalyzer::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogMumosaAI, Log, TEXT("MumosaAIAnalyzer initialized"));
}

void UMumosaAIAnalyzer::RequestAnalysis(const FString& ObjectLabel, const FString& Question)
{
	FMumosaSceneContext Context = BuildSceneContext(ObjectLabel);
	CaptureScreenshotAndSend(ObjectLabel, Question, Context);
}

void UMumosaAIAnalyzer::CaptureScreenshotAndSend(const FString& ObjectLabel, const FString& Question, const FMumosaSceneContext& Context)
{
	int32 Width = 0, Height = 0;
	TArray<FColor> Pixels = CaptureSceneToPixels(Width, Height);

	if (Width == 0 || Height == 0 || Pixels.Num() == 0)
	{
		UE_LOG(LogMumosaAI, Error, TEXT("Failed to capture scene pixels"));
		OnAnalysisComplete.Broadcast(TEXT("Analysis failed: capture error"), EMumosaConfidenceLevel::Pending, ObjectLabel);
		return;
	}

	// Debug validation: variance-based blank detection (never abort on nonWhite alone)
	{
		int64 SumR = 0, SumG = 0, SumB = 0;
		uint8 MinBright = 255, MaxBright = 0;
		for (const FColor& P : Pixels)
		{
			SumR += P.R;
			SumG += P.G;
			SumB += P.B;
			uint8 Bright = (uint8)((int32(P.R) + int32(P.G) + int32(P.B)) / 3);
			if (Bright < MinBright) MinBright = Bright;
			if (Bright > MaxBright) MaxBright = Bright;
		}
		int32 Total = Pixels.Num();
		float AvgR = (Total > 0) ? (float(SumR) / Total) : 0.0f;
		float AvgG = (Total > 0) ? (float(SumG) / Total) : 0.0f;
		float AvgB = (Total > 0) ? (float(SumB) / Total) : 0.0f;
		float AvgLum = (AvgR + AvgG + AvgB) / 3.0f;

		double VarianceSum = 0.0;
		for (const FColor& P : Pixels)
		{
			float Lum = (float)(P.R + P.G + P.B) / 3.0f;
			double Diff = (double)Lum - (double)AvgLum;
			VarianceSum += Diff * Diff;
		}
		float LumStdDev = FMath::Sqrt((float)(VarianceSum / (double)Total));
		int32 BrightRange = (int32)MaxBright - (int32)MinBright;

		UE_LOG(LogMumosaAI, Log, TEXT("Capture debug: %dx%d, avgRGB=(%.0f,%.0f,%.0f), lumStdDev=%.1f, range=%d"),
			Width, Height, AvgR, AvgG, AvgB, LumStdDev, BrightRange);

		// Only abort if truly uniform blank: avg very white + near-zero variance + no range
		if (AvgLum > 252.0f && LumStdDev < 1.5f && BrightRange < 15)
		{
			UE_LOG(LogMumosaAI, Error, TEXT("Image is blank (avgLum=%.1f, stdDev=%.1f, range=%d). Aborting."),
				AvgLum, LumStdDev, BrightRange);
			OnAnalysisComplete.Broadcast(TEXT("RETRY"), EMumosaConfidenceLevel::Pending, ObjectLabel);
			return;
		}
		if (AvgLum > 248.0f && LumStdDev < 4.0f)
		{
			UE_LOG(LogMumosaAI, Warning, TEXT("Image is bright/low-contrast (avgLum=%.1f, stdDev=%.1f) but sending to Groq anyway."),
				AvgLum, LumStdDev);
		}
	}

	TArray64<uint8> PNGData = EncodePNG(Pixels, Width, Height);
	if (PNGData.Num() == 0)
	{
		UE_LOG(LogMumosaAI, Error, TEXT("Failed to encode PNG"));
		return;
	}

	FString Base64 = Base64Encode(PNGData);
	UE_LOG(LogMumosaAI, Log, TEXT("Screenshot: %dx%d, PNG=%d bytes, Base64=%d chars"), Width, Height, PNGData.Num(), Base64.Len());

	FString DebugPath = FPaths::ProjectSavedDir() / TEXT("debug_screenshot.png");
	FFileHelper::SaveArrayToFile(PNGData, *DebugPath);
	UE_LOG(LogMumosaAI, Log, TEXT("Debug PNG saved: %s"), *DebugPath);

	SendToGroq(Base64, ObjectLabel, Question, Context);
}

TArray<FColor> UMumosaAIAnalyzer::CaptureSceneToPixels(int32& OutWidth, int32& OutHeight)
{
	UWorld* World = GetWorld();
	if (!World) return {};

	APlayerController* PC = World->GetFirstPlayerController();
	if (!PC) return {};

	APlayerCameraManager* CamMgr = PC->PlayerCameraManager;
	if (!CamMgr) return {};

	// Create capture component lazily on a hidden actor in the world
	if (!CaptureComponent)
	{
		CaptureRenderTarget = NewObject<UTextureRenderTarget2D>(GetWorld());
		CaptureRenderTarget->InitCustomFormat(1024, 576, PF_B8G8R8A8, false);
		CaptureRenderTarget->UpdateResourceImmediate(true);

		AActor* CaptureActor = World->SpawnActor<AActor>(AActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		CaptureActor->SetActorHiddenInGame(true);

		CaptureComponent = NewObject<USceneCaptureComponent2D>(CaptureActor);
		CaptureComponent->TextureTarget = CaptureRenderTarget;
		CaptureComponent->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
		CaptureComponent->bCaptureEveryFrame = false;
		CaptureComponent->bCaptureOnMovement = false;
		CaptureComponent->bHiddenInGame = false;
		CaptureActor->AddInstanceComponent(CaptureComponent);
		CaptureComponent->RegisterComponent();

		UE_LOG(LogMumosaAI, Log, TEXT("SceneCapture component created on actor"));
	}

	// Position capture at camera
	FVector CamLoc = CamMgr->GetCameraLocation();
	FRotator CamRot = CamMgr->GetCameraRotation();
	CaptureComponent->SetWorldLocationAndRotation(CamLoc, CamRot);
	CaptureComponent->FOVAngle = CamMgr->GetFOVAngle();

	// Capture
	CaptureComponent->CaptureScene();
	FlushRenderingCommands();

	// Read pixels from render target
	OutWidth = CaptureRenderTarget->SizeX;
	OutHeight = CaptureRenderTarget->SizeY;
	TArray<FColor> Pixels;
	FTextureRenderTargetResource* RTResource = CaptureRenderTarget->GameThread_GetRenderTargetResource();
	if (RTResource)
	{
		Pixels.SetNum(OutWidth * OutHeight);
		RTResource->ReadPixels(Pixels, FReadSurfaceDataFlags());
	}

	UE_LOG(LogMumosaAI, Log, TEXT("SceneCapture: %dx%d pixels read=%d"), OutWidth, OutHeight, Pixels.Num());
	return Pixels;
}

TArray64<uint8> UMumosaAIAnalyzer::EncodePNG(const TArray<FColor>& Pixels, int32 Width, int32 Height, int32 TargetSize)
{
	if (Pixels.Num() == 0) return {};

	TArray<FColor> Resized = Pixels;
	int32 ResizedW = Width;
	int32 ResizedH = Height;

	if (Width > TargetSize || Height > TargetSize)
	{
		ResizedW = TargetSize;
		ResizedH = (Height * TargetSize) / Width;
		if (ResizedH > TargetSize) { ResizedH = TargetSize; ResizedW = (Width * TargetSize) / Height; }

		TArray<FColor> Scaled;
		Scaled.Reserve(ResizedW * ResizedH);
		for (int32 Y = 0; Y < ResizedH; ++Y)
		{
			for (int32 X = 0; X < ResizedW; ++X)
			{
				int32 SrcX = (X * Width) / ResizedW;
				int32 SrcY = (Y * Height) / ResizedH;
				Scaled.Add(Pixels[SrcY * Width + SrcX]);
			}
		}
		Resized = Scaled;
	}

	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	TSharedPtr<IImageWrapper> PNGWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);
	if (!PNGWrapper.IsValid()) return {};

	if (!PNGWrapper->SetRaw(Resized.GetData(), Resized.Num() * sizeof(FColor), ResizedW, ResizedH, ERGBFormat::BGRA, 8))
	{
		UE_LOG(LogMumosaAI, Error, TEXT("Failed to set raw image data"));
		return {};
	}

	return PNGWrapper->GetCompressed(100);
}

FString UMumosaAIAnalyzer::Base64Encode(const TArray64<uint8>& Data)
{
	return FBase64::Encode(Data.GetData(), static_cast<int32>(Data.Num()));
}

void UMumosaAIAnalyzer::SendToGroq(const FString& ImageBase64, const FString& ObjectLabel, const FString& Question, const FMumosaSceneContext& Context)
{
	if (GroqApiKey.IsEmpty())
	{
		UE_LOG(LogMumosaAI, Error, TEXT("GroqApiKey is not set. Configure in DefaultGame.ini or set via Blueprint."));
		return;
	}

	// Use a vision-capable model that supports base64 images
	FString Model = TEXT("meta-llama/llama-4-scout-17b-16e-instruct");

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(TEXT("https://api.groq.com/openai/v1/chat/completions"));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *GroqApiKey));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	FString HumanName = HumanizeLabel(ObjectLabel);
	FString Prompt = FString::Printf(TEXT(
		"You are MUMOSA, a concise VR forensic assistant inside a reconstructed environment. "
		"Use the screenshot first. Use scene context and recent history to connect clues to the room. "
		"Do not mention raw Unreal asset names. "
		"Do not say \"selected object\" or \"object labeled.\" "
		"Connect the visible clue to the surrounding environment and what it suggests about how the space was used. "
		"Avoid generic phrases like \"illicit activity\" or \"unauthorized access.\" "
		"Avoid weak phrases like \"appears to be,\" \"seems to be,\" and \"the selected object.\" "
		"Write directly and naturally. "
		"Do not claim suspects or crimes unless visible evidence directly shows them. "
		"Answer in 1-2 short sentences under 45 words. "
		"No bullet points. No report format."
		" Player is looking at: %s"
		" Relevant scene context: %s"
		" Recent evidence history: %s"
		" Player question: %s"),
		*HumanName, *Context.RelevantActors,
		*Context.RecentHistory, *Question);

	// Escape JSON special chars in prompt
	FString EscapedPrompt = Prompt;
	EscapedPrompt.ReplaceInline(TEXT("\\"), TEXT("\\\\"));
	EscapedPrompt.ReplaceInline(TEXT("\""), TEXT("\\\""));
	EscapedPrompt.ReplaceInline(TEXT("\n"), TEXT("\\n"));
	EscapedPrompt.ReplaceInline(TEXT("\r"), TEXT("\\r"));
	EscapedPrompt.ReplaceInline(TEXT("\t"), TEXT("\\t"));

	FString ImageUrl = FString::Printf(TEXT("data:image/png;base64,%s"), *ImageBase64);

	FString JsonBody;
	JsonBody.Reserve(ImageBase64.Len() + 1000);
	JsonBody += TEXT("{\"model\":\"");
	JsonBody += Model;
	JsonBody += TEXT("\",\"messages\":[{\"role\":\"user\",\"content\":[{\"type\":\"text\",\"text\":\"");
	JsonBody += EscapedPrompt;
	JsonBody += TEXT("\"},{\"type\":\"image_url\",\"image_url\":{\"url\":\"");
	JsonBody += ImageUrl;
	JsonBody += TEXT("\"}}]}],\"temperature\":0.2,\"max_completion_tokens\":80}");

	UE_LOG(LogMumosaAI, Log, TEXT("Sending to Groq: model=%s, prompt=%d chars, image=%d chars, body=%d chars"),
		*Model, Prompt.Len(), ImageBase64.Len(), JsonBody.Len());

	Request->SetContentAsString(JsonBody);
	Request->OnProcessRequestComplete().BindUObject(this, &UMumosaAIAnalyzer::OnGroqResponse, ObjectLabel);
	Request->ProcessRequest();
}

void UMumosaAIAnalyzer::OnGroqResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, FString ObjectLabel)
{
	if (!bWasSuccessful || !Response.IsValid())
	{
		UE_LOG(LogMumosaAI, Error, TEXT("Groq request failed"));
		OnAnalysisComplete.Broadcast(TEXT("Analysis failed: network error"), EMumosaConfidenceLevel::Pending, ObjectLabel);
		return;
	}

	FString ResponseBody = Response->GetContentAsString();
	int32 Code = Response->GetResponseCode();
	UE_LOG(LogMumosaAI, Log, TEXT("Groq HTTP %d, body (%d chars): %s"), Code, ResponseBody.Len(), *ResponseBody.Left(500));

	if (Code != 200)
	{
		UE_LOG(LogMumosaAI, Error, TEXT("Groq HTTP error %d: %s"), Code, *ResponseBody.Left(500));
		OnAnalysisComplete.Broadcast(FString::Printf(TEXT("Analysis failed: HTTP %d"), Code), EMumosaConfidenceLevel::Pending, ObjectLabel);
		return;
	}

	TSharedPtr<FJsonObject> Json;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseBody);
	if (!FJsonSerializer::Deserialize(Reader, Json) || !Json.IsValid())
	{
		UE_LOG(LogMumosaAI, Error, TEXT("Failed to parse Groq response: %s"), *ResponseBody.Left(500));
		OnAnalysisComplete.Broadcast(TEXT("Analysis failed: parse error"), EMumosaConfidenceLevel::Pending, ObjectLabel);
		return;
	}

	if (Json->HasField(TEXT("error")))
	{
		FString ErrorMsg = Json->GetObjectField(TEXT("error"))->GetStringField(TEXT("message"));
		UE_LOG(LogMumosaAI, Error, TEXT("Groq API error: %s"), *ErrorMsg);
		OnAnalysisComplete.Broadcast(FString::Printf(TEXT("Analysis failed: %s"), *ErrorMsg), EMumosaConfidenceLevel::Pending, ObjectLabel);
		return;
	}

	const TArray<TSharedPtr<FJsonValue>>* ChoicesPtr;
	if (!Json->TryGetArrayField(TEXT("choices"), ChoicesPtr) || ChoicesPtr->Num() == 0)
	{
		UE_LOG(LogMumosaAI, Error, TEXT("Groq response has no choices field"));
		OnAnalysisComplete.Broadcast(TEXT("Analysis failed: no response"), EMumosaConfidenceLevel::Pending, ObjectLabel);
		return;
	}
	TArray<TSharedPtr<FJsonValue>> Choices = *ChoicesPtr;

	FString Content = Choices[0]->AsObject()->GetObjectField(TEXT("message"))->GetStringField(TEXT("content"));

	// Clean response: remove markdown, collapse whitespace, truncate
	Content.ReplaceInline(TEXT("**"), TEXT(""));
	Content.ReplaceInline(TEXT("\r\n"), TEXT(" "));
	Content.ReplaceInline(TEXT("\n"), TEXT(" "));
	Content.ReplaceInline(TEXT("\r"), TEXT(" "));
	while (Content.Contains(TEXT("  ")))
		Content.ReplaceInline(TEXT("  "), TEXT(" "));
	Content.TrimStartAndEndInline();

	// Safe grammar fix only
	Content.ReplaceInline(TEXT("a interior"), TEXT("an interior"), ESearchCase::IgnoreCase);

	if (Content.Len() > 220)
		Content = Content.Left(217) + TEXT("...");

	EMumosaConfidenceLevel Confidence = ParseConfidence(Content);

	OnAnalysisComplete.Broadcast(Content, Confidence, ObjectLabel);

	// Track recent history
	if (!Content.Contains(TEXT("Analysis failed")))
	{
		RecentHistory.Add(Content.Left(120));
		if (RecentHistory.Num() > MaxRecentHistory)
			RecentHistory.RemoveAt(0);
	}
}

EMumosaConfidenceLevel UMumosaAIAnalyzer::ParseConfidence(const FString& Text)
{
	FString Lower = Text.ToLower();
	if (Lower.Contains(TEXT("[confidence: 5]")) || Lower.Contains(TEXT("[confidence:5]")))
		return EMumosaConfidenceLevel::High;
	if (Lower.Contains(TEXT("[confidence: 4]")) || Lower.Contains(TEXT("[confidence:4]")))
		return EMumosaConfidenceLevel::High;
	if (Lower.Contains(TEXT("[confidence: 3]")) || Lower.Contains(TEXT("[confidence:3]")))
		return EMumosaConfidenceLevel::Medium;
	if (Lower.Contains(TEXT("[confidence: 2]")) || Lower.Contains(TEXT("[confidence:2]")))
		return EMumosaConfidenceLevel::Low;
	if (Lower.Contains(TEXT("[confidence: 1]")) || Lower.Contains(TEXT("[confidence:1]")))
		return EMumosaConfidenceLevel::Low;
	return EMumosaConfidenceLevel::Pending;
}

void UMumosaAIAnalyzer::BuildSceneIndex()
{
	UWorld* World = GetWorld();
	if (!World) return;

	SceneIndex.Reset();
	TArray<FName> SkippedClasses = {
		TEXT("Light"), TEXT("Volume"), TEXT("NavMesh"), TEXT("Sky"), TEXT("PostProcess"),
		TEXT("Collision"), TEXT("PlayerStart"), TEXT("Trigger"), TEXT("CameraRig"),
		TEXT("Fog"), TEXT("Reflection"), TEXT("DirectionalLight"), TEXT("SpotLight"),
		TEXT("PointLight"), TEXT("RectLight"), TEXT("SkyLight"), TEXT("ExponentialHeightFog"),
		TEXT("SkyAtmosphere"), TEXT("VolumetricCloud"), TEXT("PostProcessVolume"),
		TEXT("NavMeshBoundsVolume"), TEXT("BlockingVolume"), TEXT("KillZVolume"),
		TEXT("DefaultPawn"), TEXT("PlayerController"), TEXT("PlayerState"),
		TEXT("GameState"), TEXT("GameMode"), TEXT("HUD"), TEXT("SkySphere"),
		TEXT("Note"), TEXT("DocumentationActor"), TEXT("TargetPoint"),
		TEXT("CineCameraActor"), TEXT("CameraActor")
	};

	for (TActorIterator<AActor> It(World); It; ++It)
	{
		AActor* Actor = *It;
		if (!Actor || Actor->IsHidden()) continue;

		FString ClassName = Actor->GetClass()->GetName();
		bool bSkip = false;
		for (const FName& Skip : SkippedClasses)
		{
			if (ClassName.Contains(Skip.ToString()))
			{
				bSkip = true;
				break;
			}
		}
		if (bSkip) continue;

		// Skip very far actors (>50m from origin, likely sky/culling helpers)
		if (Actor->GetActorLocation().SizeSquared() > 2500000000.0f) continue;

		FMumosaSceneActor Entry;
		Entry.Label = Actor->GetActorLabel();
		Entry.ClassName = ClassName;
		Entry.Location = Actor->GetActorLocation();
		Entry.Tags = Actor->Tags;
		SceneIndex.Add(MoveTemp(Entry));
	}

	bSceneIndexBuilt = true;
	UE_LOG(LogMumosaAI, Log, TEXT("Scene index built: %d actors indexed"), SceneIndex.Num());
}

FMumosaSceneContext UMumosaAIAnalyzer::BuildSceneContext(const FString& ObjectLabel)
{
	if (!bSceneIndexBuilt) BuildSceneIndex();

	FMumosaSceneContext Context;

	// Get player camera for visibility/front-facing checks
	UWorld* World = GetWorld();
	FVector CamLoc = FVector::ZeroVector;
	FVector CamForward = FVector::ForwardVector;
	APlayerController* PC = World ? World->GetFirstPlayerController() : nullptr;
	if (PC)
	{
		if (APlayerCameraManager* Cam = PC->PlayerCameraManager)
		{
			CamLoc = Cam->GetCameraLocation();
			CamForward = Cam->GetActorForwardVector();
		}
	}

	// Find hit actor location
	FVector HitPoint = CamLoc + CamForward * 300.0f; // default ahead
	{
		FHitResult Hit;
		if (PC && PC->GetHitResultUnderCursor(ECC_Visibility, false, Hit))
		{
			HitPoint = Hit.Location;
		}
	}

	// Score and sort
	for (FMumosaSceneActor& Entry : SceneIndex)
	{
		Entry.DistanceToHit = FVector::Dist(Entry.Location, HitPoint);
		int32 Score = 0;

		// Hit/selected actor gets max priority
		if (Entry.Label.Equals(ObjectLabel, ESearchCase::IgnoreCase))
			Score += 1000;

		// Distance score (closer = higher, max 500)
		Score += FMath::Clamp(500 - FMath::RoundToInt(Entry.DistanceToHit / 2.0f), 0, 500);

		// In front of camera
		FVector ToActor = (Entry.Location - CamLoc).GetSafeNormal();
		float Dot = FVector::DotProduct(ToActor, CamForward);
		if (Dot > 0.0f) Score += FMath::RoundToInt(Dot * 100.0f);

		// Semantic tag bonus
		static const TArray<FString> ImportantTags = {
			TEXT("graffiti"), TEXT("weapon"), TEXT("door"), TEXT("camera"),
			TEXT("body"), TEXT("blood"), TEXT("debris"), TEXT("document"),
			TEXT("terminal"), TEXT("chair"), TEXT("exit"), TEXT("damage"),
			TEXT("evidence"), TEXT("clue"), TEXT("hazard"), TEXT("rubble"),
			TEXT("mark"), TEXT("note"), TEXT("heater"), TEXT("cabinet"),
			TEXT("machine"), TEXT("equipment"), TEXT("broken"), TEXT("fire")
		};
		for (const FName& Tag : Entry.Tags)
		{
			for (const FString& ImpTag : ImportantTags)
			{
				if (Tag.ToString().Contains(ImpTag))
					Score += 200;
			}
		}
		for (const FString& ImpTag : ImportantTags)
		{
			if (Entry.Label.Contains(ImpTag) || Entry.ClassName.Contains(ImpTag))
				Score += 200;
		}

		Entry.RelevanceScore = Score;
	}

	// Sort by score descending
	SceneIndex.Sort([](const FMumosaSceneActor& A, const FMumosaSceneActor& B) {
		return A.RelevanceScore > B.RelevanceScore;
	});

	// Take top 15
	const int32 MaxActors = 15;
	TArray<FString> ActorLines;
	for (int32 i = 0; i < FMath::Min(SceneIndex.Num(), MaxActors); ++i)
	{
		const FMumosaSceneActor& Entry = SceneIndex[i];
		if (Entry.RelevanceScore <= 0) break;
		FString Dir = (Entry.Location - HitPoint).GetSafeNormal().Dot(CamForward) > 0 ? TEXT("ahead") : TEXT("behind");
		ActorLines.Add(FString::Printf(TEXT("- %s (%.0fm %s)"), *Entry.Label, Entry.DistanceToHit / 100.0f, *Dir));
	}
	Context.RelevantActors = FString::Join(ActorLines, TEXT(", "));

	// Scene summary — heuristic from top actor types
	{
		TMap<FString, int32> TypeCounts;
		for (const FMumosaSceneActor& Entry : SceneIndex)
		{
			FString Type = Entry.ClassName;
			if (Type.Contains(TEXT("Wall"))) TypeCounts.FindOrAdd(TEXT("walls"))++;
			else if (Type.Contains(TEXT("Floor"))) TypeCounts.FindOrAdd(TEXT("floors"))++;
			else if (Type.Contains(TEXT("Ceiling"))) TypeCounts.FindOrAdd(TEXT("ceilings"))++;
			else if (Type.Contains(TEXT("Door"))) TypeCounts.FindOrAdd(TEXT("doors"))++;
			else if (Type.Contains(TEXT("Chair")) || Type.Contains(TEXT("Seat"))) TypeCounts.FindOrAdd(TEXT("seating"))++;
			else if (Type.Contains(TEXT("Debris")) || Type.Contains(TEXT("Rubble"))) TypeCounts.FindOrAdd(TEXT("debris"))++;
			else if (Type.Contains(TEXT("Heater")) || Type.Contains(TEXT("Radiator"))) TypeCounts.FindOrAdd(TEXT("heaters"))++;
			else if (Type.Contains(TEXT("Cabinet")) || Type.Contains(TEXT("Locker"))) TypeCounts.FindOrAdd(TEXT("cabinets"))++;
			else if (Type.Contains(TEXT("Pipe")) || Type.Contains(TEXT("Conduit"))) TypeCounts.FindOrAdd(TEXT("pipes"))++;
			else if (Type.Contains(TEXT("Wire")) || Type.Contains(TEXT("Cable"))) TypeCounts.FindOrAdd(TEXT("wiring"))++;
			else if (Type.Contains(TEXT("Light"))) TypeCounts.FindOrAdd(TEXT("lights"))++;
			else TypeCounts.FindOrAdd(TEXT("misc"))++;
		}
		TArray<FString> SummaryParts;
		for (const auto& Pair : TypeCounts)
			SummaryParts.Add(FString::Printf(TEXT("%d %s"), Pair.Value, *Pair.Key));
		Context.SceneSummary = TEXT("Abandoned industrial interior with ") + FString::Join(SummaryParts, TEXT(", ")) + TEXT(".");
	}

	// Recent history
	Context.RecentHistory = RecentHistory.Num() > 0
		? FString::Join(RecentHistory, TEXT(" | "))
		: TEXT("(none)");

	return Context;
}

FString UMumosaAIAnalyzer::HumanizeLabel(const FString& RawLabel)
{
	FString Label = RawLabel;

	// Specific overrides
	if (Label.Contains(TEXT("graffiti"), ESearchCase::IgnoreCase)) return TEXT("graffiti on wall");
	if (Label.Contains(TEXT("chair"), ESearchCase::IgnoreCase) || Label.Contains(TEXT("seat"), ESearchCase::IgnoreCase)) return TEXT("seating");
	if (Label.Contains(TEXT("debris"), ESearchCase::IgnoreCase) || Label.Contains(TEXT("rubble"), ESearchCase::IgnoreCase)) return TEXT("debris pile");
	if (Label.Contains(TEXT("heater"), ESearchCase::IgnoreCase) || Label.Contains(TEXT("radiator"), ESearchCase::IgnoreCase)) return TEXT("heater");
	if (Label.Contains(TEXT("pipe"), ESearchCase::IgnoreCase) || Label.Contains(TEXT("conduit"), ESearchCase::IgnoreCase)) return TEXT("pipes");
	if (Label.Contains(TEXT("door"), ESearchCase::IgnoreCase)) return TEXT("doorway");
	if (Label.Contains(TEXT("cabinet"), ESearchCase::IgnoreCase) || Label.Contains(TEXT("locker"), ESearchCase::IgnoreCase)) return TEXT("cabinet");
	if (Label.Contains(TEXT("wire"), ESearchCase::IgnoreCase) || Label.Contains(TEXT("cable"), ESearchCase::IgnoreCase)) return TEXT("wiring");
	if (Label.Contains(TEXT("wall"), ESearchCase::IgnoreCase)) return TEXT("wall");
	if (Label.Contains(TEXT("floor"), ESearchCase::IgnoreCase)) return TEXT("floor");
	if (Label.Contains(TEXT("ceiling"), ESearchCase::IgnoreCase)) return TEXT("ceiling");
	if (Label.Contains(TEXT("light"), ESearchCase::IgnoreCase)) return TEXT("light fixture");

	// Strip prefixes
	if (Label.StartsWith(TEXT("SM_"))) Label.RightChopInline(3);
	else if (Label.StartsWith(TEXT("BP_"))) Label.RightChopInline(3);
	else if (Label.StartsWith(TEXT("SK_"))) Label.RightChopInline(3);

	// Remove trailing numbers/underscores
	while (Label.Len() > 0 && (FChar::IsDigit(Label[Label.Len() - 1]) || Label[Label.Len() - 1] == '_'))
		Label.LeftChopInline(1);
	while (Label.Len() > 0 && Label[0] == '_')
		Label.RightChopInline(1);

	// Replace underscores with spaces
	Label.ReplaceInline(TEXT("_"), TEXT(" "));

	// To lowercase
	if (!Label.IsEmpty())
	{
		Label[0] = FChar::ToLower(Label[0]);
	}

	Label.TrimStartAndEndInline();
	if (Label.IsEmpty()) return TEXT("object");
	return Label;
}
