#include "MumosaPlayerController.h"
#include "Integration/MumosaAIAnalyzer.h"
#include "UI/MumosaEvidenceWidget.h"
#include "Evidence/MumosaEvidenceMarkerActor.h"
#include "Engine/GameInstance.h"
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

	if (CurrentPopupActor)
	{
		OnPopupClose();
		return;
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
			ShowPopup(Label, Hit.Location);
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

void AMumosaPlayerController::ShowPopup(const FString& ObjectLabel, const FVector& WorldLocation)
{
	if (!PopupActorClass || !GetWorld()) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector PopupLocation = WorldLocation;
	if (APlayerCameraManager* Cam = PlayerCameraManager)
	{
		FVector CamLoc = Cam->GetCameraLocation();
		FVector Dir = (CamLoc - PopupLocation).GetSafeNormal();
		PopupLocation += Dir * 150.0f;
		PopupLocation.Z += 80.0f;
	}

	CurrentPopupActor = GetWorld()->SpawnActor<AActor>(PopupActorClass, PopupLocation, FRotator::ZeroRotator, SpawnParams);
	if (!CurrentPopupActor) return;

	UWidgetComponent* WidgetComp = CurrentPopupActor->FindComponentByClass<UWidgetComponent>();
	if (!WidgetComp) return;

	WidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComp->SetDrawAtDesiredSize(false);
	WidgetComp->SetDrawSize(FIntPoint(500, 350));
	WidgetComp->SetBlendMode(EWidgetBlendMode::Transparent);
	WidgetComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WidgetComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	WidgetComp->SetPivot(FVector2D(0.5f, 0.5f));

	UMumosaEvidenceWidget* PopupWidget = Cast<UMumosaEvidenceWidget>(WidgetComp->GetWidget());
	if (!PopupWidget) return;

	PopupWidget->SetDisplayedObject(ObjectLabel);
	PopupWidget->OnCloseClicked.AddDynamic(this, &AMumosaPlayerController::OnPopupClose);
	PopupWidget->OnSubmitClicked.AddDynamic(this, &AMumosaPlayerController::OnPopupSubmit);

	bPendingAICapture = true;
	AICaptureRetries = 0;
	PendingAICaptureLabel = ObjectLabel;
	PendingAICaptureQuestion = TEXT("What is this object and what evidence does it provide?");
	PendingAICaptureDelay = 0.2f;
}

void AMumosaPlayerController::OnPopupClose()
{
	if (CurrentPopupActor)
	{
		CurrentPopupActor->Destroy();
		CurrentPopupActor = nullptr;
	}
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
		if (CurrentPopupActor)
		{
			UWidgetComponent* WidgetComp = CurrentPopupActor->FindComponentByClass<UWidgetComponent>();
			if (WidgetComp)
			{
				if (UMumosaEvidenceWidget* PopupWidget = Cast<UMumosaEvidenceWidget>(WidgetComp->GetWidget()))
					PopupWidget->OnAnalysisResult(TEXT("Analysis failed: could not capture view"), Confidence, ObjectLabel);
			}
		}
		AICaptureRetries = 0;
		return;
	}

	AICaptureRetries = 0;

	if (!CurrentPopupActor) return;

	UWidgetComponent* WidgetComp = CurrentPopupActor->FindComponentByClass<UWidgetComponent>();
	if (!WidgetComp) return;

	UMumosaEvidenceWidget* PopupWidget = Cast<UMumosaEvidenceWidget>(WidgetComp->GetWidget());
	if (!PopupWidget) return;

	PopupWidget->OnAnalysisResult(ResponseText, Confidence, ObjectLabel);
}