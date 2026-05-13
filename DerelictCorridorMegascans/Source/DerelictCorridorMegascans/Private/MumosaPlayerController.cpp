#include "MumosaPlayerController.h"
#include "Integration/MumosaAIAnalyzer.h"
#include "UI/MumosaEvidenceWidget.h"
#include "Evidence/MumosaEvidenceMarkerActor.h"
#include "Engine/GameInstance.h"
#include "Engine/GameViewportClient.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "UObject/Class.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/UObjectGlobals.h"
#include "Components/PrimitiveComponent.h"
#include "Components/PostProcessComponent.h"
#include "UI/MumosaFloatingPanelActor.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Styling/SlateBrush.h"
#include "Styling/SlateTypes.h"

static FString SanitizeAnalysisPopupText(const FString& InText)
{
	FString Clean;
	Clean.Reserve(InText.Len());

	for (const TCHAR Char : InText)
	{
		const bool bAllowedControl = Char == TEXT('\n') || Char == TEXT('\r') || Char == TEXT('\t');
		if (bAllowedControl || (Char >= 32 && Char != 0xffff))
		{
			Clean.AppendChar(Char);
		}
	}

	return Clean;
}

bool AMumosaPlayerController::ShouldUseViewportAnalysisPopup() const
{
	return !bUseWorldAnalysisPopup || bUseViewportPopupFallback;
}

AMumosaPlayerController::AMumosaPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = false;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	static ConstructorHelpers::FClassFinder<UMumosaEvidenceWidget> WidgetBP(TEXT("/Game/UI/WBP_EvidenceOverlay.WBP_EvidenceOverlay_C"));
	if (WidgetBP.Succeeded())
	{
		EvidenceWidgetClass = WidgetBP.Class;
	}

	static ConstructorHelpers::FClassFinder<AActor> PopupBP(TEXT("/Game/Blueprints/BP_PopupActor.BP_PopupActor_C"));
	if (PopupBP.Succeeded())
	{
		PopupActorClass = PopupBP.Class;
	}

	static ConstructorHelpers::FClassFinder<AActor> SkyBP(TEXT("/Game/Blueprints/BP_FloatingSkyChatBox.BP_FloatingSkyChatBox_C"));
	if (SkyBP.Succeeded())
	{
		SkyChatboxClass = SkyBP.Class;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> OutlineMat(TEXT("/Game/MUMOSA/Materials/MI_Outline_Orange.MI_Outline_Orange"));
	if (OutlineMat.Succeeded())
	{
		OutlineMaterial = OutlineMat.Object;
	}
}

void AMumosaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!OutlineMaterial)
	{
		OutlineMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/MUMOSA/Materials/MI_Outline_Orange.MI_Outline_Orange"));
	}

	if (OutlineMaterial && GetWorld())
	{
		OutlinePPActor = GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		if (OutlinePPActor)
		{
			UPostProcessComponent* PPComp = NewObject<UPostProcessComponent>(OutlinePPActor, TEXT("OutlinePP"));
			PPComp->bEnabled = false;
			PPComp->bUnbound = true;
			FWeightedBlendable Blendable;
			Blendable.Weight = 1.0f;
			Blendable.Object = OutlineMaterial;
			PPComp->Settings.WeightedBlendables.Array.Add(Blendable);
			OutlinePPActor->AddInstanceComponent(PPComp);
			PPComp->RegisterComponent();
		}
	}

	BindToAnalyzer();
	SpawnSkyChatbox();
}

void AMumosaPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TraceForHover();

	if (bPendingAICapture)
	{
		PendingAICaptureDelay -= DeltaTime;
		if (PendingAICaptureDelay <= 0.0f)
		{
			bPendingAICapture = false;
			if (UMumosaAIAnalyzer* Analyzer = GetGameInstance()->GetSubsystem<UMumosaAIAnalyzer>())
			{
				UE_LOG(LogTemp, Log, TEXT("MUMOSA: triggering capture attempt %d/3"), AICaptureRetries + 1);
				Analyzer->RequestAnalysis(PendingAICaptureLabel, PendingAICaptureQuestion);
				if (ShouldUseViewportAnalysisPopup())
				{
					ShowAnalysisPopupText(TEXT("Analyzing..."));
				}
			}
		}
	}

	// Face sky chatbox toward camera + floating bob
	if (SkyChatboxActor && PlayerCameraManager)
	{
		FVector CamLoc = PlayerCameraManager->GetCameraLocation();
		FRotator LookAt = (CamLoc - SkyChatboxActor->GetActorLocation()).Rotation();
		LookAt.Pitch = 0.0f;
		LookAt.Roll = 0.0f;
		SkyChatboxActor->SetActorRotation(LookAt);

		SkyChatboxTime += DeltaTime;
		FVector Loc = SkyChatboxActor->GetActorLocation();
		Loc.Z = SkyChatboxOrigin.Z + FMath::Sin(SkyChatboxTime * 0.5f) * 15.0f;
		SkyChatboxActor->SetActorLocation(Loc);
	}

	if (WasInputKeyJustPressed(EKeys::LeftMouseButton))
	{
		OnInteract();
	}
}

void AMumosaPlayerController::OnInteract()
{
	UE_LOG(LogTemp, Log, TEXT("MUMOSA OnInteract called"));

	bPendingAICapture = false;
	bPopupActive = false;
	HideAnalysisPopup();

	if (CurrentPopupActor)
	{
		CurrentPopupActor->Destroy();
		CurrentPopupActor = nullptr;
	}

	FVector2D MousePos;
	if (!GetMousePosition(MousePos.X, MousePos.Y)) return;

	FHitResult Hit;
	if (GetHitResultAtScreenPosition(MousePos, ECC_Visibility, true, Hit))
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			FString Label = HitActor->GetActorLabel();
			UE_LOG(LogTemp, Warning, TEXT("MUMOSA Hit: %s [%s]"), *Label, *HitActor->GetClass()->GetName());

			SelectedObjectLabel = Label;
			ShowPopup(Label, Hit.Location, Hit.Normal);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MUMOSA No hit at mouse"));
	}
}

void AMumosaPlayerController::SpawnSkyChatbox()
{
	if (!SkyChatboxClass || !GetWorld()) return;

	SkyChatboxOrigin = FVector(9500.0f, -2000.0f, 600.0f);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SkyChatboxActor = GetWorld()->SpawnActor<AActor>(SkyChatboxClass, SkyChatboxOrigin, FRotator::ZeroRotator, SpawnParams);
}

void AMumosaPlayerController::TraceForHover()
{
	FVector2D MousePos;
	if (!GetMousePosition(MousePos.X, MousePos.Y)) return;

	FHitResult Hit;
	if (GetHitResultAtScreenPosition(MousePos, ECC_Visibility, true, Hit))
	{
		AActor* HitActor = Hit.GetActor();
		UPrimitiveComponent* HitComp = Hit.Component.Get();

		if (HitActor && HitComp && HitActor != HoveredActor.Get())
		{
			ClearHover();
			HoveredActor = HitActor;
			HoveredComponent = HitComp;
			ApplyHighlight(HitComp);
		}
	}
	else if (HoveredActor.IsValid())
	{
		ClearHover();
	}
}

void AMumosaPlayerController::ClearHover()
{
	if (HoveredComponent.IsValid())
	{
		RemoveHighlight(HoveredComponent.Get());
	}
	HoveredActor = nullptr;
	HoveredComponent = nullptr;
}

void AMumosaPlayerController::ApplyHighlight(UPrimitiveComponent* Component)
{
	if (!Component) return;
	Component->SetRenderCustomDepth(true);
	Component->SetCustomDepthStencilValue(1);
	if (OutlinePPActor)
	{
		UPostProcessComponent* PPComp = OutlinePPActor->FindComponentByClass<UPostProcessComponent>();
		if (PPComp) PPComp->bEnabled = true;
	}
}

void AMumosaPlayerController::RemoveHighlight(UPrimitiveComponent* Component)
{
	if (!Component) return;
	Component->SetRenderCustomDepth(false);
	Component->SetCustomDepthStencilValue(0);
	if (OutlinePPActor)
	{
		UPostProcessComponent* PPComp = OutlinePPActor->FindComponentByClass<UPostProcessComponent>();
		if (PPComp) PPComp->bEnabled = false;
	}
}

void AMumosaPlayerController::ShowPopup(const FString& ObjectLabel, const FVector& WorldLocation, const FVector& HitNormal)
{
	if (!GetWorld()) return;

	if (bUseWorldAnalysisPopup)
	{
		SpawnWorldAnalysisPopup(WorldLocation, HitNormal);
	}

	if (ShouldUseViewportAnalysisPopup())
	{
		ShowAnalysisPopupText(TEXT("Analyzing..."));
		UE_LOG(LogTemp, Warning, TEXT("MUMOSA: Viewport analysis popup shown for %s"), *ObjectLabel);
	}

	bPendingAICapture = true;
	AICaptureRetries = 0;
	PendingAICaptureLabel = ObjectLabel;
	PendingAICaptureQuestion = TEXT("What is this object and what evidence does it provide?");
	PendingAICaptureDelay = 0.2f;
}

void AMumosaPlayerController::SpawnWorldAnalysisPopup(const FVector& WorldLocation, const FVector& HitNormal)
{
	if (!GetWorld()) return;

	const FVector SurfaceNormal = HitNormal.IsNearlyZero() ? FVector::UpVector : HitNormal.GetSafeNormal();
	FVector AnchoredLocation = WorldLocation + SurfaceNormal * 28.0f + FVector(0.0f, 0.0f, 32.0f);
	FVector PopupLocation = AnchoredLocation;
	FRotator PopupRotation = FRotator::ZeroRotator;
	bool bUsingCameraFallback = false;

	if (PlayerCameraManager)
	{
		const FVector CamLoc = PlayerCameraManager->GetCameraLocation();
		const FVector CamForward = PlayerCameraManager->GetCameraRotation().Vector();
		const FVector CameraFallbackLocation = CamLoc + CamForward * 220.0f + FVector(0.0f, 0.0f, -15.0f);
		const float CameraDistance = FVector::Distance(CamLoc, AnchoredLocation);

		FVector2D ScreenPosition = FVector2D::ZeroVector;
		const bool bProjected = ProjectWorldLocationToScreen(AnchoredLocation, ScreenPosition, true);
		int32 ViewX = 0;
		int32 ViewY = 0;
		GetViewportSize(ViewX, ViewY);
		const bool bViewportValid = ViewX > 0 && ViewY > 0;
		const bool bOnScreen =
			bProjected &&
			bViewportValid &&
			ScreenPosition.X >= 80.0f &&
			ScreenPosition.Y >= 80.0f &&
			ScreenPosition.X <= static_cast<float>(ViewX) - 80.0f &&
			ScreenPosition.Y <= static_cast<float>(ViewY) - 80.0f;

		bUsingCameraFallback = !bOnScreen || CameraDistance < 120.0f || CameraDistance > 360.0f;
		PopupLocation = bUsingCameraFallback ? CameraFallbackLocation : AnchoredLocation;
		PopupRotation = (CamLoc - PopupLocation).Rotation();
		PopupRotation.Pitch = 0.0f;
		PopupRotation.Roll = 0.0f;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	CurrentPopupActor = GetWorld()->SpawnActor<AMumosaFloatingPanelActor>(AMumosaFloatingPanelActor::StaticClass(), PopupLocation, PopupRotation, SpawnParams);
	if (!CurrentPopupActor)
	{
		UE_LOG(LogTemp, Error, TEXT("MUMOSA: Failed to spawn world analysis popup actor"));
		return;
	}

	CurrentPopupActor->Tags.AddUnique(TEXT("MUMOSA_WorldPopup"));
#if WITH_EDITOR
	CurrentPopupActor->SetActorLabel(TEXT("MUMOSA_WorldPopup"));
#endif

	if (AMumosaFloatingPanelActor* PanelActor = Cast<AMumosaFloatingPanelActor>(CurrentPopupActor))
	{
		PanelActor->SetAnchorData(WorldLocation, SurfaceNormal);
		PanelActor->SetActorLocation(PopupLocation);
		PanelActor->SetActorRotation(PopupRotation);
		PanelActor->SetTitleText(TEXT("MUMOSA\nAnalyzing..."));
		PanelActor->SetBodyText(TEXT("Analyzing..."));
		PanelActor->SetPopupVisible(true);
	}

	PopupBobbingOrigin = PopupLocation;
	PopupBobbingTime = 0.0f;
	bPopupActive = false;
	UE_LOG(LogTemp, Warning, TEXT("MUMOSA: World analysis popup actor staged at %s (%s, mode=%s)"), *PopupLocation.ToString(), *GetNameSafe(CurrentPopupActor), bUsingCameraFallback ? TEXT("camera_fallback") : TEXT("anchored"));
}

void AMumosaPlayerController::ShowWorldAnalysisPopupText(const FString& Text)
{
	if (!CurrentPopupActor || !IsValid(CurrentPopupActor))
	{
		return;
	}

	if (AMumosaFloatingPanelActor* PanelActor = Cast<AMumosaFloatingPanelActor>(CurrentPopupActor))
	{
		PanelActor->SetTitleText(TEXT("MUMOSA\nAnalysis"));
		PanelActor->SetBodyText(Text);
		PanelActor->SetPopupVisible(true);
		bPopupActive = true;
		UE_LOG(LogTemp, Warning, TEXT("MUMOSA: World analysis popup shown"));
	}
}

void AMumosaPlayerController::ShowAnalysisPopupText(const FString& Text)
{
	if (!ShouldUseViewportAnalysisPopup())
	{
		return;
	}

	if (!GEngine || !GEngine->GameViewport)
	{
		return;
	}

	static const FSlateRoundedBoxBrush PopupBackgroundBrush(FLinearColor(0.045f, 0.045f, 0.065f, 0.96f), 22.0f);

	if (!AnalysisPopupSlate.IsValid())
	{
		AnalysisPopupSlate =
			SNew(SOverlay)
			+ SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Top)
			.Padding(FMargin(0.0f, 145.0f, 0.0f, 0.0f))
			[
				SNew(SBox)
				.WidthOverride(520.0f)
				[
					SNew(SBorder)
					.BorderImage(&PopupBackgroundBrush)
					.Padding(FMargin(36.0f, 24.0f))
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.AutoHeight()
						.HAlign(HAlign_Center)
						.Padding(FMargin(0.0f, 0.0f, 0.0f, 18.0f))
						[
							SNew(STextBlock)
							.Text(FText::FromString(TEXT("MUMOSA\nAnalyzing...")))
							.ColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.72f, 0.02f, 1.0f)))
							.Justification(ETextJustify::Center)
							.Font(FSlateFontInfo(FCoreStyle::GetDefaultFont(), 22))
						]
						+ SVerticalBox::Slot()
						.AutoHeight()
						.HAlign(HAlign_Center)
						[
							SAssignNew(AnalysisPopupBodyText, STextBlock)
							.Text(FText::FromString(TEXT("Analyzing...")))
							.ColorAndOpacity(FSlateColor(FLinearColor::White))
							.Justification(ETextJustify::Center)
							.WrapTextAt(440.0f)
							.Font(FSlateFontInfo(FCoreStyle::GetDefaultFont(), 16))
						]
					]
				]
			];

		GEngine->GameViewport->AddViewportWidgetContent(AnalysisPopupSlate.ToSharedRef(), 1000);
		UE_LOG(LogTemp, Warning, TEXT("MUMOSA: Raw Slate analysis popup added to GameViewport"));
	}

	if (AnalysisPopupBodyText.IsValid())
	{
		AnalysisPopupBodyText->SetText(FText::FromString(SanitizeAnalysisPopupText(Text)));
	}
}

void AMumosaPlayerController::HideAnalysisPopup()
{
	if (GEngine && GEngine->GameViewport && AnalysisPopupSlate.IsValid())
	{
		GEngine->GameViewport->RemoveViewportWidgetContent(AnalysisPopupSlate.ToSharedRef());
	}

	AnalysisPopupBodyText.Reset();
	AnalysisPopupSlate.Reset();
}

void AMumosaPlayerController::OnPopupClose()
	{
	if (CurrentPopupActor)
	{
		CurrentPopupActor->Destroy();
		CurrentPopupActor = nullptr;
	}
	HideAnalysisPopup();
	bPendingAICapture = false;
	bPopupActive = false;
}

void AMumosaPlayerController::OnPopupSubmit()
{
	if (!CurrentPopupActor) return;

	UWidgetComponent* WidgetComp = CurrentPopupActor->FindComponentByClass<UWidgetComponent>();
	if (!WidgetComp) return;

	UMumosaEvidenceWidget* PopupWidget = Cast<UMumosaEvidenceWidget>(WidgetComp->GetWidget());
	if (!PopupWidget) return;

	FString Question = PopupWidget->GetQuestionText();
	if (Question.IsEmpty()) return;

	if (UMumosaAIAnalyzer* Analyzer = GetGameInstance()->GetSubsystem<UMumosaAIAnalyzer>())
	{
		Analyzer->RequestAnalysis(SelectedObjectLabel, Question);
	}
}

void AMumosaPlayerController::BindToAnalyzer()
{
	if (UMumosaAIAnalyzer* Analyzer = GetGameInstance()->GetSubsystem<UMumosaAIAnalyzer>())
	{
		Analyzer->OnAnalysisComplete.AddDynamic(this, &AMumosaPlayerController::HandleAnalysisResult);
	}
}

void AMumosaPlayerController::HandleAnalysisResult(const FString& ResponseText, EMumosaConfidenceLevel Confidence, const FString& ObjectLabel)
{
	UE_LOG(LogTemp, Log, TEXT("MUMOSA: [%s] %s"), *ObjectLabel, *ResponseText);

	// Handle retry signal from blank image detector
	if (ResponseText == TEXT("RETRY"))
	{
		AICaptureRetries++;
		if (AICaptureRetries < MaxAICaptureRetries)
		{
			bPendingAICapture = true;
			PendingAICaptureDelay = 0.05f;
			UE_LOG(LogTemp, Warning, TEXT("MUMOSA: blank capture — retry %d/%d"), AICaptureRetries + 1, MaxAICaptureRetries);
			return;
		}
		UE_LOG(LogTemp, Error, TEXT("MUMOSA: all %d captures blank. Giving up."), MaxAICaptureRetries);
		ShowWorldAnalysisPopupText(TEXT("Analysis failed: could not capture view"));
		if (ShouldUseViewportAnalysisPopup())
		{
			ShowAnalysisPopupText(TEXT("Analysis failed: could not capture view"));
		}
		AICaptureRetries = 0;
		return;
	}

	AICaptureRetries = 0;
	ShowWorldAnalysisPopupText(ResponseText);
	if (ShouldUseViewportAnalysisPopup())
	{
		ShowAnalysisPopupText(ResponseText);
	}
}
