#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UI/MumosaEvidenceWidget.h"
#include "MumosaPlayerController.generated.h"

UCLASS()
class DERELICTCORRIDORMEGASCANS_API AMumosaPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMumosaPlayerController();

	UPROPERTY(EditDefaultsOnly, Category = "MUMOSA")
	float InteractRange = 800.0f;

	UPROPERTY(EditDefaultsOnly, Category = "MUMOSA|UI")
	TSubclassOf<UMumosaEvidenceWidget> EvidenceWidgetClass;

protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnInteract();

	void PerformLineTraceAndAnalyze();

	UFUNCTION()
	void HandleAnalysisResult(const FString& ResponseText, EMumosaConfidenceLevel Confidence, const FString& ObjectLabel);

	UPROPERTY()
	TObjectPtr<UMumosaEvidenceWidget> EvidenceWidget;

	void BindToAnalyzer();
};
