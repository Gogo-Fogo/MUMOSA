#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Data/MumosaEvidenceTypes.h"
#include "Http.h"
#include "MumosaAIAnalyzer.generated.h"

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
	void CaptureScreenshotAndSend(const FString& ObjectLabel, const FString& Question);
	void ReadViewportPixels(TArray<FColor>& OutPixels, int32& OutWidth, int32& OutHeight);
	TArray64<uint8> EncodePNG(const TArray<FColor>& Pixels, int32 Width, int32 Height, int32 TargetSize = 1344);
	FString Base64Encode(const TArray64<uint8>& Data);
	void SendToGroq(const FString& ImageBase64, const FString& ObjectLabel, const FString& Question);
	void OnGroqResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, FString ObjectLabel);

	EMumosaConfidenceLevel ParseConfidence(const FString& Text);
};
