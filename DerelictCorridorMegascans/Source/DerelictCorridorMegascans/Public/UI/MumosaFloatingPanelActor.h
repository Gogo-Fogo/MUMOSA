#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MumosaFloatingPanelActor.generated.h"

class UWidgetComponent;
class UTextRenderComponent;

UCLASS()
class DERELICTCORRIDORMEGASCANS_API AMumosaFloatingPanelActor : public AActor
{
	GENERATED_BODY()

public:
	AMumosaFloatingPanelActor();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void SetBodyText(const FString& Text);
	void SetPopupVisible(bool bVisible);

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWidgetComponent> WidgetComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTextRenderComponent> TextRender_Title;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTextRenderComponent> TextRender_Body;
};