#include "MumosaPlayerController.h"
#include "Integration/MumosaAIAnalyzer.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Blueprint/UserWidget.h"
#include "UObject/Class.h"

AMumosaPlayerController::AMumosaPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
}

void AMumosaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (EvidenceWidgetClass)
	{
		EvidenceWidget = CreateWidget<UMumosaEvidenceWidget>(this, EvidenceWidgetClass);
		if (EvidenceWidget)
		{
			EvidenceWidget->AddToViewport();
		}
	}

	BindToAnalyzer();
}

void AMumosaPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (InputComponent)
	{
		InputComponent->BindAction("Interact", IE_Pressed, this, &AMumosaPlayerController::OnInteract);
	}
}

void AMumosaPlayerController::OnInteract()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("Interact pressed — analyzing..."));
	PerformLineTraceAndAnalyze();
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
	FString ConfidenceStr = StaticEnum<EMumosaConfidenceLevel>()->GetDisplayNameTextByValue((int64)Confidence).ToString();
	FString Msg = FString::Printf(TEXT("[%s] %s — %s"), *ObjectLabel, *ConfidenceStr, *ResponseText);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, Msg);
	UE_LOG(LogTemp, Log, TEXT("MUMOSA: %s"), *Msg);

	if (EvidenceWidget)
	{
		EvidenceWidget->OnAnalysisResult(ResponseText, Confidence, ObjectLabel);
	}
}

void AMumosaPlayerController::PerformLineTraceAndAnalyze()
{
	FVector CamLoc;
	FRotator CamRot;
	GetPlayerViewPoint(CamLoc, CamRot);

	FVector End = CamLoc + CamRot.Vector() * InteractRange;
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetPawn());

	if (GetWorld()->LineTraceSingleByChannel(Hit, CamLoc, End, ECC_Visibility, Params))
	{
		AActor* HitActor = Hit.GetActor();
		if (!HitActor) return;

		FString Label = HitActor->GetActorLabel();
		if (Label.IsEmpty()) Label = HitActor->GetName();

		if (UMumosaAIAnalyzer* Analyzer = GetGameInstance()->GetSubsystem<UMumosaAIAnalyzer>())
		{
			Analyzer->RequestAnalysis(Label, TEXT("What is this object and what evidence does it provide?"));
		}
	}
}
