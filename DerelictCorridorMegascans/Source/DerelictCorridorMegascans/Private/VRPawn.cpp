#include "VRPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "Components/DecalComponent.h"
#include "MotionControllerComponent.h"
#include "Engine/World.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Evidence/MumosaEvidenceMarkerActor.h"
#include "MumosaPlayerController.h"
#include "DrawDebugHelpers.h"

AVRPawn::AVRPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VRRoot"));
	RootComponent = VRRoot;

	VROrigin = CreateDefaultSubobject<USceneComponent>(TEXT("VROrigin"));
	VROrigin->SetupAttachment(VRRoot);

	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	VRCamera->SetupAttachment(VROrigin);
	VRCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 64.0f));
	VRCamera->bUsePawnControlRotation = true;

	LeftMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftMotionController"));
	LeftMotionController->SetupAttachment(VROrigin);
	LeftMotionController->MotionSource = FName("LeftGrip");

	RightMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightMotionController"));
	RightMotionController->SetupAttachment(VROrigin);
	RightMotionController->MotionSource = FName("RightGrip");

	TeleportMarker = CreateDefaultSubobject<UDecalComponent>(TEXT("TeleportMarker"));
	TeleportMarker->SetupAttachment(RootComponent);
	TeleportMarker->DecalSize = FVector(32.0f, 32.0f, 32.0f);
	TeleportMarker->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	TeleportMarker->SetVisibility(false);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	bCanTeleport = false;
}

void AVRPawn::BeginPlay()
{
	Super::BeginPlay();
	// OpenXR handles HMD initialization automatically
}

void AVRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (VRCamera && VROrigin)
	{
		FVector CameraLocation = VRCamera->GetComponentLocation();
		FRotator CameraRotation = VRCamera->GetComponentRotation();

		FVector NewOriginLocation = FVector(CameraLocation.X, CameraLocation.Y, 0.0f);
		VROrigin->SetWorldLocation(NewOriginLocation);
		VROrigin->SetWorldRotation(CameraRotation);
	}

	if (TeleportMarker && TeleportMarker->IsVisible())
	{
		FVector MarkerLocation = TeleportMarker->GetComponentLocation();
		MarkerLocation.Z = GetActorLocation().Z + 1.0f;
		TeleportMarker->SetWorldLocation(MarkerLocation);
	}
}

void AVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

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
	AttemptInteraction(LeftMotionController);
}

void AVRPawn::OnLeftGripReleased()
{
	UE_LOG(LogTemp, Warning, TEXT("Left Grip Released"));
}

void AVRPawn::OnRightGripPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("Right Grip Pressed"));
	AttemptInteraction(RightMotionController);
}

void AVRPawn::OnRightGripReleased()
{
	UE_LOG(LogTemp, Warning, TEXT("Right Grip Released"));
}

void AVRPawn::OnLeftTriggerPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("Left Trigger Pressed"));
	StartTeleportPreview();
}

void AVRPawn::OnLeftTriggerReleased()
{
	UE_LOG(LogTemp, Warning, TEXT("Left Trigger Released"));
	CompleteTeleport();
}

void AVRPawn::OnRightTriggerPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("Right Trigger Pressed"));
}

void AVRPawn::OnRightTriggerReleased()
{
	UE_LOG(LogTemp, Warning, TEXT("Right Trigger Released"));
}

void AVRPawn::OnLeftThumbstickPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("Left Thumbstick Pressed"));
}

void AVRPawn::OnRightThumbstickPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("Right Thumbstick Pressed"));
}

void AVRPawn::AttemptInteraction(UMotionControllerComponent* MotionController)
{
	if (!MotionController || !GetWorld()) return;

	FVector Start = MotionController->GetComponentLocation();
	FVector Forward = MotionController->GetForwardVector();
	FVector End = Start + (Forward * 100.0f);

	FCollisionQueryParams TraceParams(FName(TEXT("VRInteractionTrace")), true, this);
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = false;

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, TraceParams);

	if (HitResult.bBlockingHit)
	{
		if (AMumosaEvidenceMarkerActor* Marker = Cast<AMumosaEvidenceMarkerActor>(HitResult.GetActor()))
		{
			UE_LOG(LogTemp, Warning, TEXT("VR Interaction with Marker: %s"), *Marker->GetMarkerId().ToString());

			for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
			{
				if (AMumosaPlayerController* PC = Cast<AMumosaPlayerController>(*Iterator))
				{
					if (PC->CurrentPopupActor)
					{
						PC->OnPopupClose();
					}

					PC->SelectedObjectLabel = Marker->GetMarkerId().ToString();
					if (!PC->SelectedObjectLabel.IsEmpty())
					{
						PC->ShowPopup(PC->SelectedObjectLabel, HitResult.Location);
					}
					break;
				}
			}

			Marker->SetSelected(true);
		}
	}
}

void AVRPawn::StartTeleportPreview()
{
	if (!VRCamera) return;

	FVector Start, Dir;
	if (LeftMotionController && LeftMotionController->IsTracked())
	{
		Start = LeftMotionController->GetComponentLocation();
		Dir = LeftMotionController->GetForwardVector();
	}
	else
	{
		Start = VRCamera->GetComponentLocation();
		Dir = VRCamera->GetForwardVector();
	}

	Dir.Z = 0.0f;
	Dir.Normalize();

	const float TraceDistance = 500.0f;
	FVector End = Start + (Dir * TraceDistance);

	FCollisionQueryParams TraceParams(FName(TEXT("TeleportTrace")), true, this);
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = false;

	FHitResult HitResult(ForceInit);
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, TraceParams);

	if (HitResult.bBlockingHit)
	{
		bCanTeleport = true;
		TeleportDestination = HitResult.ImpactPoint;

		if (TeleportMarker)
		{
			TeleportMarker->SetVisibility(true);
			TeleportMarker->SetWorldLocation(TeleportDestination);

			if (TeleportBlinkMaterial)
			{
				TeleportMarker->SetDecalMaterial(TeleportBlinkMaterial);
			}
		}
	}
	else
	{
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

	FVector NewLocation = TeleportDestination;
	NewLocation.Z += 10.0f;

	SetActorLocation(NewLocation);

	if (TeleportMarker)
	{
		TeleportMarker->SetVisibility(false);
	}

	bCanTeleport = false;
}
