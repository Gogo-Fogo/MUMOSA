// Fill out your copyright notice in the Description page of Project Settings.

#include "VRPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "MotionControllerComponent.h"
#include "Engine/World.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Evidence/MumosaEvidenceMarkerActor.h"
#include "UI/MumosaEvidenceWidget.h"
#include "DrawDebugHelpers.h"

// Sets default values
AVRPawn::AVRPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// VR Root
	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VRRoot"));
	RootComponent = VRRoot;

	// VR Origin (tracks the HMD)
	VROrigin = CreateDefaultSubobject<USceneComponent>(TEXT("VROrigin"));
	VROrigin->SetupAttachment(VRRoot);

	// Camera
	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	VRCamera->SetupAttachment(VROrigin);
	VRCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 64.0f)); // Eye height
	VRCamera->bUsePawnControlRotation = true;

	// Motion Controllers
	LeftMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftMotionController"));
	LeftMotionController->SetupAttachment(VROrigin);
	LeftMotionController->TrackingSource = EControllerHand::Left;
	LeftMotionController->bDisplayDeviceModel = true;

	RightMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightMotionController"));
	RightMotionController->SetupAttachment(VROrigin);
	RightMotionController->TrackingSource = EControllerHand::Right;
	RightMotionController->bDisplayDeviceModel = true;

	// Teleport Marker
	TeleportMarker = CreateDefaultSubobject<UDecalComponent>(TEXT("TeleportMarker"));
	TeleportMarker->SetupAttachment(RootComponent);
	TeleportMarker->DecalSize = FVector(32.0f, 32.0f, 32.0f);
	TeleportMarker->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f)); // Face up
	TeleportMarker->SetVisibility(false);

	// Disable character movement as we'll handle VR movement separately
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure teleport variables
	bCanTeleport = false;
}

// Called when the game starts or when spawned
void AVRPawn::BeginPlay()
{
	Super::BeginPlay();

	// Enable HMD
	if (IsValid(UHeadMountedDisplayFunctionLibrary::GetHMDDevice()))
	{
		UHeadMountedDisplayFunctionLibrary::EnableHMD(true);
	}
}

// Called every frame
void AVRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update VR origin to match HMD position and rotation
	if (VRCamera && VROrigin)
	{
		FVector CameraLocation = VRCamera->GetComponentLocation();
		FRotator CameraRotation = VRCamera->GetComponentRotation();

		// Keep the origin at floor level (zero Z) while allowing camera to move
		FVector NewOriginLocation = FVector(CameraLocation.X, CameraLocation.Y, 0.0f);
		VROrigin->SetWorldLocation(NewOriginLocation);
		VROrigin->SetWorldRotation(CameraRotation);
	}

	// Update teleport marker visibility and position
	if (TeleportMarker && TeleportMarker->IsVisible())
	{
		// Keep teleport marker facing up
		FVector MarkerLocation = TeleportMarker->GetComponentLocation();
		MarkerLocation.Z = GetActorLocation().Z + 1.0f; // Slightly above floor
		TeleportMarker->SetWorldLocation(MarkerLocation);
	}
}

// Called to bind functionality to input
void AVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind VR inputs
	PlayerInputComponent->BindAction("LeftGrip", IE_Pressed, this, &AVRPawn::OnLeftGripPressed);
	PlayerInputComponent->BindAction("LeftGrip", IE_Released, this, &AVRPawn::OnLeftGripReleased);
	PlayerInputComponent->BindAction("RightGrip", IE_Pressed, this, &AVRPawn::OnRightGripPressed);
	PlayerInputComponent->BindAction("RightGrip", IE_Released, this, &AVRPawn::OnRightGripReleased);
	PlayerInputComponent->BindAction("LeftTrigger", IE_Pressed, this, &AVRPawn::OnLeftTriggerPressed);
	PlayerInputComponent->BindAction("LeftTrigger", IE_Released, this, &AVRPawn::OnLeftTriggerReleased);
	PlayerInputComponent->BindAction("RightTrigger", IE_Pressed, this, &AVRPawn::OnRightTriggerPressed);
	PlayerInputComponent->BindAction("RightTrigger", IE_Released, this, &AVRPawn::OnRightTriggerReleased);
	PlayerInputComponent->BindAction("LeftThumbstick", IE_Pressed, this, &AVRPawn::OnLeftThumbstickPressed);
	PlayerInputComponent->BindAction("RightThumbstick", IE_Pressed, this, &AVRPawn::OnRightThumbstickPressed);
}

void AVRPawn::OnLeftGripPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("Left Grip Pressed"));
	// Attempt to interact with evidence marker via line trace
	AttemptInteraction(LeftMotionController);
}

void AVRPawn::OnLeftGripReleased()
{
	UE_LOG(LogTemp, Warning, TEXT("Left Grip Released"));
	// Release grabbed objects
}

void AVRPawn::OnRightGripPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("Right Grip Pressed"));
	// Attempt to interact with evidence marker via line trace
	AttemptInteraction(RightMotionController);
}

void AVRPawn::OnRightGripReleased()
{
	UE_LOG(LogTemp, Warning, TEXT("Right Grip Released"));
	// Release grabbed objects
}

void AVRPawn::OnLeftTriggerPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("Left Trigger Pressed"));
	// Start teleport preview
	StartTeleportPreview();
}

void AVRPawn::OnLeftTriggerReleased()
{
	UE_LOG(LogTemp, Warning, TEXT("Left Trigger Released"));
	// Complete teleport if valid
	CompleteTeleport();
}

void AVRPawn::OnRightTriggerPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("Right Trigger Pressed"));
	// Alternative action (could be grab or UI interaction)
}

void AVRPawn::OnRightTriggerReleased()
{
	UE_LOG(LogTemp, Warning, TEXT("Right Trigger Released"));
}

void AVRPawn::OnLeftThumbstickPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("Left Thumbstick Pressed"));
	// Snap turn or reset orientation
}

void AVRPawn::OnRightThumbstickPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("Right Thumbstick Pressed"));
	// Snap turn or reset orientation
}

void AVRPawn::AttemptInteraction(UMotionControllerComponent* Controller)
{
	if (!Controller || !GetWorld()) return;

	// Get controller location and forward vector
	FVector Start = Controller->GetComponentLocation();
	FVector Forward = Controller->GetForwardVector();
	FVector End = Start + (Forward * 100.0f); // 100 units reach

	// Perform line trace
	FCollisionQueryParams TraceParams(FName(TEXT("VRInteractionTrace")), true, this);
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = false;

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, TraceParams);

	// Optional: draw debug line
	//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.1f, 0, 1.0f);

	if (HitResult.bBlockingHit)
	{
		// Check if we hit an evidence marker
		if (AMumosaEvidenceMarkerActor* Marker = Cast<AMumosaEvidenceMarkerActor>(HitResult.GetActor()))
		{
			UE_LOG(LogTemp, Warning, TEXT("VR Interaction with Marker: %s"), *Marker->GetMarkerId().ToString());
			
			// Trigger the exact same popup system as desktop OnInteract()
			for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
			{
				if (AMumosaPlayerController* PC = Cast<AMumosaPlayerController>(*Iterator))
				{
					// Close any existing popup first (same as OnInteract)
					if (PC->CurrentPopupActor)
					{
						PC->OnPopupClose();
					}
					
					// Set selected object label and show popup (same as OnInteract)
					PC->SelectedObjectLabel = Marker->GetMarkerId().ToString();
					if (!PC->SelectedObjectLabel.IsEmpty()) 
					{
						PC->ShowPopup(PC->SelectedObjectLabel, HitResult.Location);
					}
					break;
				}
			}
			
			// Update marker selection state for visual feedback
			Marker->SetSelected(true);
		}
	}
}

void AVRPawn::StartTeleportPreview()
{
	if (!VRCamera) return;

	// Get forward vector from left controller or camera
	FVector Start, Dir;
	if (LeftMotionController && LeftMotionController->IsTrackingValid())
	{
		Start = LeftMotionController->GetComponentLocation();
		Dir = LeftMotionController->GetForwardVector();
	}
	else
	{
		Start = VRCamera->GetComponentLocation();
		Dir = VRCamera->GetForwardVector();
	}

	// Remove Z component for floor-level teleport
	Dir.Z = 0.0f;
	Dir.Normalize();

	// Trace forward to find walkable surface
	const float TraceDistance = 500.0f;
	FVector End = Start + (Dir * TraceDistance);

	FCollisionQueryParams TraceParams(FName(TEXT("TeleportTrace")), true, this);
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = false;

	FHitResult HitResult(ForceInit);
	GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		TraceParams
	);

	if (HitResult.bBlockingHit)
	{
		// Valid teleport destination
		bCanTeleport = true;
		TeleportDestination = HitResult.ImpactPoint;
		
		// Show teleport marker
		if (TeleportMarker)
		{
			TeleportMarker->SetVisibility(true);
			TeleportMarker->SetWorldLocation(TeleportDestination);
			
			// Optional: apply blink material if set
			if (TeleportBlinkMaterial)
			{
				TeleportMarker->SetDecalMaterial(TeleportBlinkMaterial);
			}
		}
	}
	else
	{
		// Invalid teleport destination
		bCanTeleport = false;
		if (TeleportMarker)
		{
			TeleportMarker->SetVisibility(false);
		}
	}
}

void AVRPawn::CompleteTeleport()
{
	if (!bCanTeleport) return;

	// Teleport the player
	FVector NewLocation = TeleportDestination;
	NewLocation.Z += 10.0f; // Slight lift to avoid floor penetration

	// Set player location
	SetActorLocation(NewLocation);
	
	// Hide teleport marker
	if (TeleportMarker)
	{
		TeleportMarker->SetVisibility(false);
	}
	
	bCanTeleport = false;
}