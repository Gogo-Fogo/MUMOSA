#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VRPawn.generated.h"

class UCameraComponent;
class USceneComponent;
class UMotionControllerComponent;
class UDecalComponent;
class ANavMeshBoundsVolume;
class UMaterialInterface;

UCLASS()
class DERELICTCORRIDORMEGASCANS_API AVRPawn : public APawn
{
	GENERATED_BODY()

public:
	AVRPawn();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* VRCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VR)
	USceneComponent* VRRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR|Controllers")
	UMotionControllerComponent* LeftMotionController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR|Controllers")
	UMotionControllerComponent* RightMotionController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VR)
	USceneComponent* VROrigin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR|Teleportation")
	ANavMeshBoundsVolume* NavMeshBoundsVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR|Teleportation")
	UMaterialInterface* TeleportBlinkMaterial;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR|Teleportation")
	bool bCanTeleport;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR|Teleportation")
	FVector TeleportDestination;

	UPROPERTY()
	UDecalComponent* TeleportMarker;

private:
	void OnLeftGripPressed();
	void OnLeftGripReleased();
	void OnRightGripPressed();
	void OnRightGripReleased();
	void OnLeftTriggerPressed();
	void OnLeftTriggerReleased();
	void OnRightTriggerPressed();
	void OnRightTriggerReleased();
	void OnLeftThumbstickPressed();
	void OnRightThumbstickPressed();

	void AttemptInteraction(UMotionControllerComponent* MotionController);
	void StartTeleportPreview();
	void CompleteTeleport();
};
