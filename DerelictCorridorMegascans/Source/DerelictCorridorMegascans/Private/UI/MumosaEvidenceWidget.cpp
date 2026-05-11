#include "UI/MumosaEvidenceWidget.h"
#include "Integration/MumosaAIAnalyzer.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Engine/Engine.h"

void UMumosaEvidenceWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetWidgetReferences();

	if (CloseBtn)
	{
		CloseBtn->OnClicked.AddDynamic(this, &UMumosaEvidenceWidget::OnCloseButtonClicked);
	}
	if (SubmitBtn)
	{
		SubmitBtn->OnClicked.AddDynamic(this, &UMumosaEvidenceWidget::OnSubmitButtonClicked);
	}
	if (InputTextBox)
	{
		InputTextBox->OnTextCommitted.AddDynamic(this, &UMumosaEvidenceWidget::OnTextCommitted);
	}
}

void UMumosaEvidenceWidget::NativeDestruct()
{
	if (CloseBtn)
	{
		CloseBtn->OnClicked.RemoveDynamic(this, &UMumosaEvidenceWidget::OnCloseButtonClicked);
	}
	if (SubmitBtn)
	{
		SubmitBtn->OnClicked.RemoveDynamic(this, &UMumosaEvidenceWidget::OnSubmitButtonClicked);
	}
	if (InputTextBox)
	{
		InputTextBox->OnTextCommitted.RemoveDynamic(this, &UMumosaEvidenceWidget::OnTextCommitted);
	}
	Super::NativeDestruct();
}

void UMumosaEvidenceWidget::SetWidgetReferences()
{
	TitleTextWidget = Cast<UTextBlock>(GetWidgetFromName(TEXT("TitleText")));
	ResponseBodyTextWidget = Cast<UTextBlock>(GetWidgetFromName(TEXT("ResponseBodyText")));
	InputTextBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("QuestionInput")));
	SubmitBtn = Cast<UButton>(GetWidgetFromName(TEXT("SubmitButton")));
	CloseBtn = Cast<UButton>(GetWidgetFromName(TEXT("CloseButton")));
	LoadingThrobberWidget = GetWidgetFromName(TEXT("LoadingThrobber"));
	LoadingTextWidget = Cast<UTextBlock>(GetWidgetFromName(TEXT("LoadingText")));
}

void UMumosaEvidenceWidget::SetDisplayedObject(const FString& ObjectLabel)
{
	CurrentObjectLabel = ObjectLabel;
	bPendingAnalysis = true;
	if (TitleTextWidget) TitleTextWidget->SetText(FText::FromString(ObjectLabel));
	if (ResponseBodyTextWidget) ResponseBodyTextWidget->SetText(FText::FromString(TEXT("Analyzing...")));
	if (LoadingThrobberWidget) LoadingThrobberWidget->SetVisibility(ESlateVisibility::Visible);
	if (LoadingTextWidget) LoadingTextWidget->SetVisibility(ESlateVisibility::Visible);
	if (InputTextBox)
	{
		InputTextBox->SetText(FText::GetEmpty());
		InputTextBox->SetHintText(FText::FromString(TEXT("Ask a question about this object...")));
	}
}

FString UMumosaEvidenceWidget::GetQuestionText() const
{
	if (InputTextBox) return InputTextBox->GetText().ToString();
	return FString();
}

void UMumosaEvidenceWidget::OnAnalysisStarted(const FString& ObjectLabel)
{
	if (ResponseBodyTextWidget) ResponseBodyTextWidget->SetText(FText::FromString(TEXT("Analyzing...")));
}

void UMumosaEvidenceWidget::OnAnalysisResult(const FString& ResponseText, EMumosaConfidenceLevel Confidence, const FString& ObjectLabel)
{
	if (ResponseBodyTextWidget) ResponseBodyTextWidget->SetText(FText::FromString(ResponseText));
	if (LoadingThrobberWidget) LoadingThrobberWidget->SetVisibility(ESlateVisibility::Collapsed);
	if (LoadingTextWidget) LoadingTextWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void UMumosaEvidenceWidget::OnCloseButtonClicked()
{
	OnCloseClicked.Broadcast();
}

void UMumosaEvidenceWidget::OnSubmitButtonClicked()
{
	OnSubmitClicked.Broadcast();
}

void UMumosaEvidenceWidget::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		OnSubmitClicked.Broadcast();
	}
}

void UMumosaEvidenceWidget::HandleAnalysisResult(const FString& ResponseText, EMumosaConfidenceLevel Confidence, const FString& ObjectLabel)
{
	OnAnalysisResult(ResponseText, Confidence, ObjectLabel);
}
