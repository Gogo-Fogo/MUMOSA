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

	UPROPERTY(EditDefaultsOnly, Category = "MUMOSA|UI")
	TSubclassOf<AActor> PopupActorClass;

	UPROPERTY(EditDefaultsOnly, Category = "MUMOSA|UI")
	UMaterialInterface* HighlightMaterial;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnInteract();

	void TraceForHover();
	void ClearHover();
	void ApplyHighlight(UPrimitiveComponent* Component);
	void RemoveHighlight(UPrimitiveComponent* Component);

	void ShowPopup(const FString& ObjectLabel, const FVector& WorldLocation);

	UFUNCTION()
	void OnPopupClose();

	void SpawnSkyChatbox();

	UFUNCTION()
	void OnPopupSubmit();

	UFUNCTION()
	void HandleAnalysisResult(const FString& ResponseText, EMumosaConfidenceLevel Confidence, const FString& ObjectLabel);

	UPROPERTY()
	TObjectPtr<UMumosaEvidenceWidget> EvidenceWidget;

	UPROPERTY()
	TObjectPtr<AActor> CurrentPopupActor;

	UPROPERTY(EditDefaultsOnly, Category = "MUMOSA|UI")
	TSubclassOf<AActor> SkyChatboxClass;

	UPROPERTY()
	TObjectPtr<AActor> SkyChatboxActor;

	UPROPERTY()
	TWeakObjectPtr<AActor> HoveredActor;

	UPROPERTY()
	TWeakObjectPtr<UPrimitiveComponent> HoveredComponent;

	FString SelectedObjectLabel;

	float SkyChatboxTime = 0.0f;
	FVector SkyChatboxOrigin;

	void BindToAnalyzer();
};
