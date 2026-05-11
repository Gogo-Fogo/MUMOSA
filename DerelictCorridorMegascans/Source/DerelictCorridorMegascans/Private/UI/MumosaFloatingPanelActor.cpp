#include "UI/MumosaFloatingPanelActor.h"
#include "UI/MumosaFloatingPanelWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/SceneComponent.h"
#include "Components/TextRenderComponent.h"
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

	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("MumosaPanelWidget"));
	WidgetComp->SetupAttachment(Root);
	WidgetComp->SetWidgetClass(UMumosaFloatingPanelWidget::StaticClass());
	WidgetComp->SetWidgetSpace(EWidgetSpace::World);
	WidgetComp->SetDrawSize(FIntPoint(350, 120));
	WidgetComp->SetBlendMode(EWidgetBlendMode::Opaque);
	WidgetComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WidgetComp->SetPivot(FVector2D(0.5f, 0.5f));
	WidgetComp->SetTwoSided(true);
	WidgetComp->SetVisibility(true);
	WidgetComp->SetHiddenInGame(false);
	WidgetComp->SetWorldScale3D(FVector(0.25f));

	TextRender_Title = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TitleText"));
	TextRender_Title->SetupAttachment(Root);
	TextRender_Title->SetText(FText::FromString(TEXT("MUMOSA")));
	TextRender_Title->SetWorldSize(10.0f);
	TextRender_Title->SetRelativeLocation(FVector(3.0f, 0.0f, 9.0f));
	TextRender_Title->SetTextRenderColor(FColor(255, 200, 60));
	TextRender_Title->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	TextRender_Title->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	TextRender_Title->SetHiddenInGame(false);
	TextRender_Title->SetVisibility(true);

	TextRender_Body = CreateDefaultSubobject<UTextRenderComponent>(TEXT("BodyText"));
	TextRender_Body->SetupAttachment(Root);
	TextRender_Body->SetText(FText::FromString(TEXT("Analyzing...")));
	TextRender_Body->SetWorldSize(6.5f);
	TextRender_Body->SetRelativeLocation(FVector(3.0f, 0.0f, -4.0f));
	TextRender_Body->SetTextRenderColor(FColor(255, 255, 255));
	TextRender_Body->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	TextRender_Body->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	TextRender_Body->SetHiddenInGame(false);
	TextRender_Body->SetVisibility(true);

	PrimaryActorTick.bCanEverTick = true;
}

void AMumosaFloatingPanelActor::BeginPlay()
{
	Super::BeginPlay();

	if (WidgetComp)
	{
		WidgetComp->InitWidget();
	}
}

void AMumosaFloatingPanelActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC && PC->PlayerCameraManager)
	{
		FVector CamLoc = PC->PlayerCameraManager->GetCameraLocation();
		FVector ToCam = (CamLoc - GetActorLocation());
		ToCam.Z = 0.f;
		if (!ToCam.IsNearlyZero())
		{
			SetActorRotation(ToCam.Rotation());
		}
	}
}

void AMumosaFloatingPanelActor::SetBodyText(const FString& Text)
{
	if (TextRender_Body)
	{
		FString Wrapped = WrapText(Text, 30, 3);
		TextRender_Body->SetText(FText::FromString(Wrapped));
	}
}

void AMumosaFloatingPanelActor::SetPopupVisible(bool bVisible)
{
	SetActorHiddenInGame(!bVisible);
	SetActorTickEnabled(bVisible);
}