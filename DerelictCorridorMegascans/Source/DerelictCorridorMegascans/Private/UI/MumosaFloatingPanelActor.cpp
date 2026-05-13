#include "UI/MumosaFloatingPanelActor.h"
#include "UI/MumosaFloatingPanelWidget.h"
#include "Components/SceneComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

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

	RoundedBackgroundComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("RoundedBackground"));
	RoundedBackgroundComponent->SetupAttachment(Root);
	RoundedBackgroundComponent->SetWidgetClass(UMumosaFloatingPanelWidget::StaticClass());
	RoundedBackgroundComponent->SetWidgetSpace(EWidgetSpace::World);
	RoundedBackgroundComponent->SetDrawSize(FVector2D(640.0f, 280.0f));
	RoundedBackgroundComponent->SetBlendMode(EWidgetBlendMode::Transparent);
	RoundedBackgroundComponent->SetTwoSided(true);
	RoundedBackgroundComponent->SetTickWhenOffscreen(true);
	RoundedBackgroundComponent->SetTranslucentSortPriority(100);
	RoundedBackgroundComponent->SetPivot(FVector2D(0.5f, 0.5f));
	RoundedBackgroundComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	RoundedBackgroundComponent->SetRelativeScale3D(FVector(0.18f, 0.18f, 0.18f));
	RoundedBackgroundComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PrimaryActorTick.bCanEverTick = true;
}

void AMumosaFloatingPanelActor::BeginPlay()
{
	Super::BeginPlay();

	SetActorScale3D(FVector(1.0f));
	RoundedBackgroundComponent->InitWidget();
	RoundedBackgroundComponent->SetVisibility(true, true);
	RoundedBackgroundComponent->SetHiddenInGame(false, true);
}

void AMumosaFloatingPanelActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC && PC->PlayerCameraManager)
	{
		FVector CamLoc = PC->PlayerCameraManager->GetCameraLocation();
		FVector ToCam = (CamLoc - GetActorLocation());
		if (!ToCam.IsNearlyZero())
		{
			FRotator LookAt = ToCam.Rotation();
			LookAt.Pitch = 0.0f;
			LookAt.Roll = 0.0f;
			SetActorRotation(LookAt);
		}
	}
}

void AMumosaFloatingPanelActor::SetAnchorData(const FVector& InAnchorLocation, const FVector& InSurfaceNormal)
{
	AnchorLocation = InAnchorLocation;
	SurfaceNormal = InSurfaceNormal.IsNearlyZero() ? FVector::ForwardVector : InSurfaceNormal.GetSafeNormal();

	const FVector UpOffset(0.0f, 0.0f, 32.0f);
	const FVector SurfaceOffset = SurfaceNormal * 28.0f;
	SetActorLocation(AnchorLocation + SurfaceOffset + UpOffset);
}

void AMumosaFloatingPanelActor::SetBodyText(const FString& Text)
{
	FString Wrapped = WrapText(Text, 40, 6);
	RoundedBackgroundComponent->InitWidget();

	if (UMumosaFloatingPanelWidget* RoundedWidget = Cast<UMumosaFloatingPanelWidget>(RoundedBackgroundComponent->GetUserWidgetObject()))
	{
		RoundedWidget->SetBodyText(Wrapped);
	}
}

void AMumosaFloatingPanelActor::SetTitleText(const FString& Text)
{
	RoundedBackgroundComponent->InitWidget();

	if (UMumosaFloatingPanelWidget* RoundedWidget = Cast<UMumosaFloatingPanelWidget>(RoundedBackgroundComponent->GetUserWidgetObject()))
	{
		RoundedWidget->SetTitleText(Text);
	}
}

void AMumosaFloatingPanelActor::SetPopupVisible(bool bVisible)
{
	SetActorHiddenInGame(!bVisible);
	SetActorTickEnabled(bVisible);
	RoundedBackgroundComponent->SetVisibility(bVisible, true);
	RoundedBackgroundComponent->SetHiddenInGame(!bVisible, true);
}
