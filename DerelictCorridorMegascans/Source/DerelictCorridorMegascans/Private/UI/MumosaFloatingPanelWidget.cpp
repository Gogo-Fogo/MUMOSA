#include "UI/MumosaFloatingPanelWidget.h"
#include "Components/SizeBox.h"
#include "Components/Border.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"
#include "Engine/Texture2D.h"

void UMumosaFloatingPanelWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (WidgetTree->RootWidget)
		return;

	USizeBox* Root = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("Root"));
	Root->SetWidthOverride(350.0f);
	Root->SetHeightOverride(120.0f);
	WidgetTree->RootWidget = Root;

	UTexture2D* WhiteTex = LoadObject<UTexture2D>(nullptr, TEXT("/Engine/EngineResources/WhiteSquareTexture"));

	UBorder* Background = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("Background"));
	if (WhiteTex)
	{
		Background->SetBrushFromTexture(WhiteTex);
	}
	Background->SetBrushColor(FLinearColor(0.06f, 0.06f, 0.07f, 1.0f));
	Background->SetPadding(FMargin(0.0f));
	Root->AddChild(Background);
}

void UMumosaFloatingPanelWidget::SetTitleText(const FString& Text)
{
}

void UMumosaFloatingPanelWidget::SetBodyText(const FString& Text)
{
}