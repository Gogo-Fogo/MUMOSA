#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/MumosaEvidenceTypes.h"
#include "MumosaEvidenceWidget.generated.h"

UCLASS(Abstract, BlueprintType)
class DERELICTCORRIDORMEGASCANS_API UMumosaEvidenceWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "MUMOSA|UI")
	void OnAnalysisResult(const FString& ResponseText, EMumosaConfidenceLevel Confidence, const FString& ObjectLabel);

	UFUNCTION(BlueprintImplementableEvent, Category = "MUMOSA|UI")
	void OnAnalysisStarted(const FString& ObjectLabel);

	UFUNCTION(BlueprintCallable, Category = "MUMOSA|UI")
	void RequestAnalysisFor(const FString& ObjectLabel, const FString& Question);

private:
	UFUNCTION()
	void HandleAnalysisResult(const FString& ResponseText, EMumosaConfidenceLevel Confidence, const FString& ObjectLabel);
};
