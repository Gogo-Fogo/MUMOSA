#include "Evidence/MumosaEvidenceMarkerActor.h"
#include "Components/TextRenderComponent.h"
#include "Components/SphereComponent.h"

AMumosaEvidenceMarkerActor::AMumosaEvidenceMarkerActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bSelected = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	MarkerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MarkerMesh"));
	MarkerMesh->SetupAttachment(SceneRoot);
	MarkerMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MarkerMesh->SetCollisionResponseToAllChannels(ECR_Block);

	LabelText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("LabelText"));
	LabelText->SetupAttachment(SceneRoot);
	LabelText->SetRelativeLocation(FVector(0, 0, 120));
	LabelText->SetHorizontalAlignment(EHTA_Center);
	LabelText->SetWorldSize(24);

	InteractionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionVolume"));
	InteractionVolume->SetupAttachment(SceneRoot);
	InteractionVolume->SetSphereRadius(200);
	InteractionVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractionVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionVolume->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	SetActorEnableCollision(true);
}

void AMumosaEvidenceMarkerActor::SetMarkerRecord(const FMumosaEvidenceMarkerRecord& Record)
{
	MarkerRecord = Record;
	MarkerId = Record.MarkerId;

	if (LabelText)
	{
		LabelText->SetText(FText::FromString(Record.Label));
	}
}

void AMumosaEvidenceMarkerActor::SetSelected(bool bInSelected)
{
	bSelected = bInSelected;

	if (MarkerMesh)
	{
		UMaterialInterface* Mat = bSelected ? SelectedMaterial : DefaultMaterial;
		if (Mat)
		{
			MarkerMesh->SetMaterial(0, Mat);
		}
	}
}

void AMumosaEvidenceMarkerActor::BeginPlay()
{
	Super::BeginPlay();
}

void AMumosaEvidenceMarkerActor::NotifyActorOnClicked(FKey ButtonPressed)
{
	Super::NotifyActorOnClicked(ButtonPressed);
	OnMarkerSelected.Broadcast(this);
}
