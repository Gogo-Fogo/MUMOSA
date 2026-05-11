#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Data/MumosaEvidenceTypes.h"
#include "Http.h"
#include "Engine/TextureRenderTarget2D.h"
#include "MumosaAIAnalyzer.generated.h"

class USceneCaptureComponent2D;

struct FMumosaSceneActor
{
	FString Label;
	FString ClassName;
	FVector Location;
	float DistanceToHit = 0.0f;
	TArray<FName> Tags;
	int32 RelevanceScore = 0;
};

struct FMumosaSceneContext
{
	FString RelevantActors;
	FString SceneSummary;
	FString RecentHistory;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAnalysisComplete, const FString&, ResponseText, EMumosaConfidenceLevel, Confidence, const FString&, ObjectLabel);

UCLASS(Config=Game, BlueprintType)
class DERELICTCORRIDORMEGASCANS_API UMumosaAIAnalyzer : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, Category = "MUMOSA|AI")
	void RequestAnalysis(const FString& ObjectLabel, const FString& Question);

	UPROPERTY(BlueprintAssignable, Category = "MUMOSA|AI")
	FOnAnalysisComplete OnAnalysisComplete;

	UPROPERTY(Config, EditAnywhere, Category = "MUMOSA|AI")
	FString GroqApiKey;

	UPROPERTY(Config, EditAnywhere, Category = "MUMOSA|AI")
	FString GroqModel = TEXT("meta-llama/llama-4-scout-17b-16e-instruct");

private:
	void CaptureScreenshotAndSend(const FString& ObjectLabel, const FString& Question, const FMumosaSceneContext& Context);
	TArray64<uint8> EncodePNG(const TArray<FColor>& Pixels, int32 Width, int32 Height, int32 TargetSize = 512);
	FString Base64Encode(const TArray64<uint8>& Data);
	void SendToGroq(const FString& ImageBase64, const FString& ObjectLabel, const FString& Question, const FMumosaSceneContext& Context);
	void OnGroqResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, FString ObjectLabel);

	EMumosaConfidenceLevel ParseConfidence(const FString& Text);

	TArray<FColor> CaptureSceneToPixels(int32& OutWidth, int32& OutHeight);

	void BuildSceneIndex();
	FMumosaSceneContext BuildSceneContext(const FString& ObjectLabel);
	FString HumanizeLabel(const FString& RawLabel);

	UPROPERTY()
	TObjectPtr<USceneCaptureComponent2D> CaptureComponent;

	UPROPERTY()
	TObjectPtr<UTextureRenderTarget2D> CaptureRenderTarget;

	TArray<FMumosaSceneActor> SceneIndex;
	bool bSceneIndexBuilt = false;
	TArray<FString> RecentHistory;
	static constexpr int32 MaxRecentHistory = 5;
};
