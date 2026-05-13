#pragma once

#include "CoreMinimal.h"
#include "MumosaEvidenceTypes.generated.h"

UENUM(BlueprintType)
enum class EMumosaConfidenceLevel : uint8
{
	High	UMETA(DisplayName = "High"),
	Medium	UMETA(DisplayName = "Medium"),
	Low		UMETA(DisplayName = "Low"),
	Pending	UMETA(DisplayName = "Pending")
};

UENUM(BlueprintType)
enum class EMumosaEvidenceStatus : uint8
{
	Confirmed	UMETA(DisplayName = "Confirmed"),
	Inferred	UMETA(DisplayName = "Inferred"),
	Conflicting	UMETA(DisplayName = "Conflicting"),
	Pending		UMETA(DisplayName = "Pending")
};

USTRUCT(BlueprintType)
struct FMumosaSourceRecord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MUMOSA|Evidence")
	FName SourceId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MUMOSA|Evidence")
	FString Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MUMOSA|Evidence")
	FString SourceType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MUMOSA|Evidence")
	FString Timestamp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MUMOSA|Evidence")
	FString Attribution;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MUMOSA|Evidence")
	FString Excerpt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MUMOSA|Evidence")
	FString FilePathOrUrl;
};

USTRUCT(BlueprintType)
struct FMumosaEvidenceMarkerRecord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MUMOSA|Evidence")
	FName MarkerId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MUMOSA|Evidence")
	FString Label;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MUMOSA|Evidence")
	FString Summary;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MUMOSA|Evidence")
	FString AIInterpretation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MUMOSA|Evidence")
	EMumosaConfidenceLevel Confidence = EMumosaConfidenceLevel::Pending;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MUMOSA|Evidence")
	EMumosaEvidenceStatus Status = EMumosaEvidenceStatus::Pending;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MUMOSA|Evidence")
	FString TimelineEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MUMOSA|Evidence")
	FString TimeRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MUMOSA|Evidence")
	FString DiscrepancyNote;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MUMOSA|Evidence")
	FString SuggestedNextStep;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MUMOSA|Evidence")
	TArray<FMumosaSourceRecord> Sources;
};
