#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UI/MumosaEvidenceWidget.h"
#include "MumosaPlayerController.generated.h"

class STextBlock;
class SWidget;

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
	bool bUseWorldAnalysisPopup = true;

	UPROPERTY(EditDefaultsOnly, Category = "MUMOSA|UI")
	bool bUseViewportPopupFallback = false;

	UPROPERTY(EditDefaultsOnly, Category = "MUMOSA|UI")
	UMaterialInterface* HighlightMaterial;

	void ShowPopup(const FString& ObjectLabel, const FVector& WorldLocation, const FVector& HitNormal);

	UFUNCTION()
	void OnPopupClose();

	UPROPERTY()
	TObjectPtr<AActor> CurrentPopupActor;

	FString SelectedObjectLabel;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnInteract();

	void TraceForHover();
	void ClearHover();
	void ApplyHighlight(UPrimitiveComponent* Component);
	void RemoveHighlight(UPrimitiveComponent* Component);

	void SpawnSkyChatbox();
	void SpawnWorldAnalysisPopup(const FVector& WorldLocation, const FVector& HitNormal);
	void ShowWorldAnalysisPopupText(const FString& Text);
	void ShowAnalysisPopupText(const FString& Text);
	void HideAnalysisPopup();
	bool ShouldUseViewportAnalysisPopup() const;

	UFUNCTION()
	void OnPopupSubmit();

	UFUNCTION()
	void HandleAnalysisResult(const FString& ResponseText, EMumosaConfidenceLevel Confidence, const FString& ObjectLabel);

	UPROPERTY()
	TObjectPtr<UMumosaEvidenceWidget> EvidenceWidget;

	TSharedPtr<SWidget> AnalysisPopupSlate;
	TSharedPtr<STextBlock> AnalysisPopupBodyText;

	UPROPERTY()
	TObjectPtr<UMaterialInterface> OutlineMaterial;

	UPROPERTY()
	TObjectPtr<class AActor> OutlinePPActor;

	UPROPERTY(EditDefaultsOnly, Category = "MUMOSA|UI")
	TSubclassOf<AActor> SkyChatboxClass;

	UPROPERTY()
	TObjectPtr<AActor> SkyChatboxActor;

	UPROPERTY()
	TWeakObjectPtr<AActor> HoveredActor;

	UPROPERTY()
	TWeakObjectPtr<UPrimitiveComponent> HoveredComponent;

	float SkyChatboxTime = 0.0f;
	FVector SkyChatboxOrigin;

	float PopupBobbingTime = 0.0f;
	FVector PopupBobbingOrigin;
	bool bPopupActive = false;

	void BindToAnalyzer();

	bool bPendingAICapture = false;
	FString PendingAICaptureLabel;
	FString PendingAICaptureQuestion;
	float PendingAICaptureDelay = 0.0f;
	int32 AICaptureRetries = 0;
	static constexpr int32 MaxAICaptureRetries = 3;
};
