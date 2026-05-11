#include "UI/MumosaFloatingPanelActor.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Core/ActorComponent/UIText.h"

static FString WrapText(const FString& InText, int32 MaxCharsPerLine, int32 MaxLines)
{
	if (InText.IsEmpty()) return InText;

	TArray<FString> Words;
	InText.ParseIntoArray(Words, TEXT(" "), true);

	FString Result;
	FString CurrentLine;
	int32 LineCount = 0;

	for (const FString& Word : Words)
	{
		FString TestLine = CurrentLine.IsEmpty() ? Word : CurrentLine + TEXT(" ") + Word;
		if (TestLine.Len() > MaxCharsPerLine && !CurrentLine.IsEmpty())
		{
			LineCount++;
			if (LineCount >= MaxLines)
			{
				if (!Result.IsEmpty()) Result += TEXT("\n");
				Result += CurrentLine;
				Result += TEXT("...");
				return Result;
			}
			if (!Result.IsEmpty()) Result += TEXT("\n");
			Result += CurrentLine;
			CurrentLine = Word;
		}
		else
		{
			CurrentLine = TestLine;
		}
	}

	if (!CurrentLine.IsEmpty())
	{
		if (!Result.IsEmpty()) Result += TEXT("\n");
		Result += CurrentLine;
	}

	return Result;
}

AMumosaFloatingPanelActor::AMumosaFloatingPanelActor()
{
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	static ConstructorHelpers::FClassFinder<AActor> LGUIBP(TEXT("/Game/MUMOSA/UI/BP_MumosaLGUIFloatingPanel.BP_MumosaLGUIFloatingPanel_C"));
	if (LGUIBP.Succeeded())
	{
		LGUIPopupClass = LGUIBP.Class;
	}

	PrimaryActorTick.bCanEverTick = true;
}

void AMumosaFloatingPanelActor::BeginPlay()
{
	Super::BeginPlay();

	if (LGUIPopupClass && GetWorld() && !LGUIPanelActor)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		LGUIPanelActor = GetWorld()->SpawnActor<AActor>(LGUIPopupClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if (LGUIPanelActor)
		{
			LGUIPanelActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		}
	}
}

void AMumosaFloatingPanelActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC && PC->PlayerCameraManager)
	{
		FVector CamLoc = PC->PlayerCameraManager->GetCameraLocation();
		FVector FromCam = (GetActorLocation() - CamLoc);
		FromCam.Z = 0.f;
		if (!FromCam.IsNearlyZero())
		{
			SetActorRotation(FromCam.Rotation());
		}
	}
}

void AMumosaFloatingPanelActor::SetBodyText(const FString& Text)
{
	if (!LGUIPanelActor) return;

	FString Wrapped = WrapText(Text, 40, 6);

	TArray<UUIText*> TextComps;
	LGUIPanelActor->GetComponents<UUIText>(TextComps);
	for (UUIText* Comp : TextComps)
	{
		if (Comp->GetName().Contains(TEXT("BodyText")))
		{
			Comp->SetText(FText::FromString(Wrapped));
			return;
		}
	}
}

void AMumosaFloatingPanelActor::SetTitleText(const FString& Text)
{
	if (!LGUIPanelActor) return;

	TArray<UUIText*> TextComps;
	LGUIPanelActor->GetComponents<UUIText>(TextComps);
	for (UUIText* Comp : TextComps)
	{
		if (Comp->GetName().Contains(TEXT("TitleText")))
		{
			Comp->SetText(FText::FromString(Text));
			return;
		}
	}
}

void AMumosaFloatingPanelActor::SetPopupVisible(bool bVisible)
{
	SetActorHiddenInGame(!bVisible);
	SetActorTickEnabled(bVisible);
	if (LGUIPanelActor)
	{
		LGUIPanelActor->SetActorHiddenInGame(!bVisible);
	}
}
