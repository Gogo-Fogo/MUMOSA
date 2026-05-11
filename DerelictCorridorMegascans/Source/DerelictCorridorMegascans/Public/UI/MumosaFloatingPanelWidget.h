#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MumosaFloatingPanelWidget.generated.h"

class USizeBox;
class UBorder;
class UVerticalBox;
class UTextBlock;

UCLASS()
class DERELICTCORRIDORMEGASCANS_API UMumosaFloatingPanelWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void SetTitleText(const FString& Text);
	void SetBodyText(const FString& Text);

private:
	UPROPERTY()
	TObjectPtr<UTextBlock> TitleText;

	UPROPERTY()
	TObjectPtr<UTextBlock> BodyText;
};
