#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/MumosaEvidenceTypes.h"
#include "MumosaEvidenceWidget.generated.h"

class UTextBlock;
class UEditableTextBox;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWidgetClose);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWidgetSubmit);

UCLASS(Abstract, BlueprintType)
class DERELICTCORRIDORMEGASCANS_API UMumosaEvidenceWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintCallable, Category = "MUMOSA|UI")
	void OnAnalysisResult(const FString& ResponseText, EMumosaConfidenceLevel Confidence, const FString& ObjectLabel);

	UFUNCTION(BlueprintCallable, Category = "MUMOSA|UI")
	void OnAnalysisStarted(const FString& ObjectLabel);

	UFUNCTION(BlueprintCallable, Category = "MUMOSA|UI")
	void SetDisplayedObject(const FString& ObjectLabel);

	UFUNCTION(BlueprintCallable, Category = "MUMOSA|UI")
	FString GetQuestionText() const;

	UPROPERTY(BlueprintAssignable, Category = "MUMOSA|UI")
	FOnWidgetClose OnCloseClicked;

	UPROPERTY(BlueprintAssignable, Category = "MUMOSA|UI")
	FOnWidgetSubmit OnSubmitClicked;

private:
	void SetWidgetReferences();

	UFUNCTION()
	void HandleAnalysisResult(const FString& ResponseText, EMumosaConfidenceLevel Confidence, const FString& ObjectLabel);

	UFUNCTION()
	void OnCloseButtonClicked();

	UFUNCTION()
	void OnSubmitButtonClicked();

	UFUNCTION()
	void OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	UPROPERTY()
	TObjectPtr<UTextBlock> TitleTextWidget;

	UPROPERTY()
	TObjectPtr<UTextBlock> ResponseBodyTextWidget;

	UPROPERTY()
	TObjectPtr<UEditableTextBox> InputTextBox;

	UPROPERTY()
	TObjectPtr<UButton> SubmitBtn;

	UPROPERTY()
	TObjectPtr<UButton> CloseBtn;

	bool bPendingAnalysis = false;
	FString CurrentObjectLabel;
};
