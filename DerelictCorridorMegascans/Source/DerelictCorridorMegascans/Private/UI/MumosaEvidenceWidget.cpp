#include "UI/MumosaEvidenceWidget.h"
#include "Integration/MumosaAIAnalyzer.h"

void UMumosaEvidenceWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (UMumosaAIAnalyzer* Analyzer = GetGameInstance()->GetSubsystem<UMumosaAIAnalyzer>())
	{
		Analyzer->OnAnalysisComplete.AddDynamic(this, &UMumosaEvidenceWidget::HandleAnalysisResult);
	}
}

void UMumosaEvidenceWidget::NativeDestruct()
{
	if (UMumosaAIAnalyzer* Analyzer = GetGameInstance()->GetSubsystem<UMumosaAIAnalyzer>())
	{
		Analyzer->OnAnalysisComplete.RemoveDynamic(this, &UMumosaEvidenceWidget::HandleAnalysisResult);
	}
	Super::NativeDestruct();
}

void UMumosaEvidenceWidget::RequestAnalysisFor(const FString& ObjectLabel, const FString& Question)
{
	OnAnalysisStarted(ObjectLabel);
	if (UMumosaAIAnalyzer* Analyzer = GetGameInstance()->GetSubsystem<UMumosaAIAnalyzer>())
	{
		Analyzer->RequestAnalysis(ObjectLabel, Question);
	}
}

void UMumosaEvidenceWidget::HandleAnalysisResult(const FString& ResponseText, EMumosaConfidenceLevel Confidence, const FString& ObjectLabel)
{
	OnAnalysisResult(ResponseText, Confidence, ObjectLabel);
}
