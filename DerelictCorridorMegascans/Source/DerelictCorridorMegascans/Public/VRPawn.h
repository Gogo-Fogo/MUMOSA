// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VRPawn.generated.h"

UCLASS()
class DERELICTCORRIDORMEGASCANS_API AVRPawn : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this pawn's properties
	AVRPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Camera for VR */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* VRCamera;

	/** Root component for VR */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VR)
	class USceneComponent* VRRoot;

	/** Motion controller components */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR|Controllers")
	class UMotionControllerComponent* LeftMotionController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR|Controllers")
	class UMotionControllerComponent* RightMotionController;

	/** VR Origin (camera root) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VR)
	class USceneComponent* VROrigin;

	/** Teleportation-related properties */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR|Teleportation")
	class UNavMeshBoundsVolume* NavMeshBoundsVolume;

	/** Teleport blink material */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR|Teleportation")
	class UMaterialInterface* TeleportBlinkMaterial;

	/** Whether we can currently teleport */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR|Teleportation")
	bool bCanTeleport;

	/** Teleport destination */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR|Teleportation")
	FVector TeleportDestination;

	/** Current location marker for teleport */
	UPROPERTY()
	class UDecalComponent* TeleportMarker;
};