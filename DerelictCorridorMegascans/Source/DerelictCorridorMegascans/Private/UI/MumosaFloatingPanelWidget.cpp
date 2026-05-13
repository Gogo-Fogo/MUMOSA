#include "UI/MumosaFloatingPanelWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/SizeBox.h"
#include "Components/Border.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"
#include "Styling/SlateBrush.h"
#include "Styling/SlateTypes.h"

static FString SanitizePopupText(const FString& InText)
{
	FString Clean;
	Clean.Reserve(InText.Len());

	for (const TCHAR Char : InText)
	{
		const bool bAllowedControl = Char == TEXT('\n') || Char == TEXT('\r') || Char == TEXT('\t');
		if (bAllowedControl || Char >= 32)
		{
			Clean.AppendChar(Char);
		}
	}

	return Clean;
}

void UMumosaFloatingPanelWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (WidgetTree->RootWidget && TitleText && BodyText)
		return;

	UCanvasPanel* RootCanvas = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("RootCanvas"));
	WidgetTree->RootWidget = RootCanvas;

	USizeBox* Root = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("PopupRoot"));
	Root->SetWidthOverride(420.0f);
	Root->SetHeightOverride(150.0f);

	if (UCanvasPanelSlot* CanvasSlot = RootCanvas->AddChildToCanvas(Root))
	{
		CanvasSlot->SetAnchors(FAnchors(0.5f, 0.42f));
		CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
		CanvasSlot->SetAutoSize(true);
		CanvasSlot->SetPosition(FVector2D::ZeroVector);
	}

	UBorder* Background = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("Background"));
	const FSlateRoundedBoxBrush BackgroundBrush(FLinearColor(0.045f, 0.045f, 0.065f, 0.96f), 22.0f);
	Background->SetBrush(BackgroundBrush);
	Background->SetPadding(FMargin(30.0f, 22.0f));
	Root->AddChild(Background);

	UVerticalBox* Layout = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("Layout"));
	Background->SetContent(Layout);

	TitleText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("TitleText"));
	TitleText->SetText(FText::FromString(TEXT("MUMOSA\nAnalyzing...")));
	TitleText->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.72f, 0.02f, 1.0f)));
	TitleText->SetJustification(ETextJustify::Center);
	TitleText->SetFont(FSlateFontInfo(FCoreStyle::GetDefaultFont(), 18));

	if (UVerticalBoxSlot* TitleSlot = Layout->AddChildToVerticalBox(TitleText))
	{
		TitleSlot->SetHorizontalAlignment(HAlign_Fill);
		TitleSlot->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 16.0f));
	}

	BodyText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("BodyText"));
	BodyText->SetText(FText::FromString(TEXT("Analyzing...")));
	BodyText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	BodyText->SetJustification(ETextJustify::Center);
	BodyText->SetAutoWrapText(true);
	BodyText->SetWrapTextAt(340.0f);
	BodyText->SetFont(FSlateFontInfo(FCoreStyle::GetDefaultFont(), 15));

	if (UVerticalBoxSlot* BodySlot = Layout->AddChildToVerticalBox(BodyText))
	{
		BodySlot->SetHorizontalAlignment(HAlign_Fill);
	}
}

void UMumosaFloatingPanelWidget::SetTitleText(const FString& Text)
{
	if (TitleText)
	{
		TitleText->SetText(FText::FromString(SanitizePopupText(Text)));
	}
}

void UMumosaFloatingPanelWidget::SetBodyText(const FString& Text)
{
	if (BodyText)
	{
		BodyText->SetText(FText::FromString(SanitizePopupText(Text)));
	}
}
