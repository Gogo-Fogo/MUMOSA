#include "Integration/MumosaAIAnalyzer.h"
#include "Engine/GameViewportClient.h"
#include "Slate/SceneViewport.h"
#include "ImageUtils.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Misc/Base64.h"
#include "Serialization/BufferArchive.h"

DEFINE_LOG_CATEGORY_STATIC(LogMumosaAI, Log, All);

void UMumosaAIAnalyzer::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogMumosaAI, Log, TEXT("MumosaAIAnalyzer initialized"));
}

void UMumosaAIAnalyzer::RequestAnalysis(const FString& ObjectLabel, const FString& Question)
{
	CaptureScreenshotAndSend(ObjectLabel, Question);
}

void UMumosaAIAnalyzer::CaptureScreenshotAndSend(const FString& ObjectLabel, const FString& Question)
{
	TArray<FColor> Pixels;
	int32 Width = 0, Height = 0;
	ReadViewportPixels(Pixels, Width, Height);

	if (Width == 0 || Height == 0 || Pixels.Num() == 0)
	{
		UE_LOG(LogMumosaAI, Error, TEXT("Failed to capture viewport pixels"));
		return;
	}

	TArray64<uint8> PNGData = EncodePNG(Pixels, Width, Height);
	if (PNGData.Num() == 0)
	{
		UE_LOG(LogMumosaAI, Error, TEXT("Failed to encode PNG"));
		return;
	}

	FString Base64 = Base64Encode(PNGData);
	SendToGroq(Base64, ObjectLabel, Question);
}

void UMumosaAIAnalyzer::ReadViewportPixels(TArray<FColor>& OutPixels, int32& OutWidth, int32& OutHeight)
{
	UWorld* World = GetWorld();
	if (!World) return;

	UGameViewportClient* ViewportClient = World->GetGameViewport();
	if (!ViewportClient) return;

	FViewport* Viewport = ViewportClient->Viewport;
	if (!Viewport) return;

	OutWidth = Viewport->GetSizeXY().X;
	OutHeight = Viewport->GetSizeXY().Y;
	if (OutWidth <= 0 || OutHeight <= 0) return;

	Viewport->ReadPixels(OutPixels);
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

	if (!PNGWrapper->SetRaw(Resized.GetData(), Resized.Num() * sizeof(FColor), ResizedW, ResizedH, ERGBFormat::RGBA, 8))
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

void UMumosaAIAnalyzer::SendToGroq(const FString& ImageBase64, const FString& ObjectLabel, const FString& Question)
{
	if (GroqApiKey.IsEmpty())
	{
		UE_LOG(LogMumosaAI, Error, TEXT("GroqApiKey is not set. Configure in DefaultGame.ini or set via Blueprint."));
		return;
	}

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(TEXT("https://api.groq.com/openai/v1/chat/completions"));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *GroqApiKey));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	FString Prompt = FString::Printf(TEXT(
		"You are a forensic investigator analyzing a real environment captured via photogrammetry. "
		"Ignore any UI elements. Focus on the 3D content as real-world objects. "
		"Inspecting: %s Question: %s "
		"End with [Confidence: 1-5]."),
		*ObjectLabel, *Question);

	FString Body = FString::Printf(TEXT(
		R"({
			"model": "%s",
			"messages": [{"role": "user", "content": [{"type": "text", "text": "%s"}, {"type": "image_url", "image_url": {"url": "data:image/png;base64,%s"}}]}],
			"max_tokens": 800
		})"),
		*GroqModel, *Prompt, *ImageBase64);

	Request->SetContentAsString(Body);
	Request->OnProcessRequestComplete().BindUObject(this, &UMumosaAIAnalyzer::OnGroqResponse, ObjectLabel);
	Request->ProcessRequest();
}

void UMumosaAIAnalyzer::OnGroqResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, FString ObjectLabel)
{
	if (!bWasSuccessful || !Response.IsValid())
	{
		UE_LOG(LogMumosaAI, Error, TEXT("Groq request failed"));
		OnAnalysisComplete.Broadcast(TEXT("[Analysis failed]"), EMumosaConfidenceLevel::Pending, ObjectLabel);
		return;
	}

	FString ResponseBody = Response->GetContentAsString();
	UE_LOG(LogMumosaAI, Log, TEXT("Groq response body: %s"), *ResponseBody);

	TSharedPtr<FJsonObject> Json;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseBody);
	if (!FJsonSerializer::Deserialize(Reader, Json) || !Json.IsValid())
	{
		UE_LOG(LogMumosaAI, Error, TEXT("Failed to parse Groq response"));
		OnAnalysisComplete.Broadcast(TEXT("[Parse error]"), EMumosaConfidenceLevel::Pending, ObjectLabel);
		return;
	}

	if (Json->HasField(TEXT("error")))
	{
		FString ErrorMsg = Json->GetObjectField(TEXT("error"))->GetStringField(TEXT("message"));
		UE_LOG(LogMumosaAI, Error, TEXT("Groq API error: %s"), *ErrorMsg);
		OnAnalysisComplete.Broadcast(FString::Printf(TEXT("[Groq error: %s]"), *ErrorMsg), EMumosaConfidenceLevel::Pending, ObjectLabel);
		return;
	}

	const TArray<TSharedPtr<FJsonValue>>* ChoicesPtr;
	if (!Json->TryGetArrayField(TEXT("choices"), ChoicesPtr) || ChoicesPtr->Num() == 0)
	{
		UE_LOG(LogMumosaAI, Error, TEXT("Groq response has no choices field"));
		OnAnalysisComplete.Broadcast(TEXT("[No response]"), EMumosaConfidenceLevel::Pending, ObjectLabel);
		return;
	}
	TArray<TSharedPtr<FJsonValue>> Choices = *ChoicesPtr;

	FString Content = Choices[0]->AsObject()->GetObjectField(TEXT("message"))->GetStringField(TEXT("content"));
	EMumosaConfidenceLevel Confidence = ParseConfidence(Content);

	OnAnalysisComplete.Broadcast(Content, Confidence, ObjectLabel);
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
