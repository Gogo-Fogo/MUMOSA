#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MumosaFloatingPanelActor.generated.h"

UCLASS()
class DERELICTCORRIDORMEGASCANS_API AMumosaFloatingPanelActor : public AActor
{
	GENERATED_BODY()

public:
	AMumosaFloatingPanelActor();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void SetBodyText(const FString& Text);
	void SetTitleText(const FString& Text);
	void SetPopupVisible(bool bVisible);

protected:
	UPROPERTY()
	TSubclassOf<AActor> LGUIPopupClass;

public:
	UPROPERTY()
	TObjectPtr<AActor> LGUIPanelActor;
};
