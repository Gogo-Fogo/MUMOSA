#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/MumosaEvidenceTypes.h"
#include "MumosaEvidenceMarkerActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMarkerSelected, AMumosaEvidenceMarkerActor*, Marker);

UCLASS()
class MUMOSASPATIAL_API AMumosaEvidenceMarkerActor : public AActor
{
	GENERATED_BODY()

public:
	AMumosaEvidenceMarkerActor();

	UPROPERTY(BlueprintAssignable, Category = "MUMOSA|Evidence")
	FOnMarkerSelected OnMarkerSelected;

	UFUNCTION(BlueprintCallable, Category = "MUMOSA|Evidence")
	void SetMarkerRecord(const FMumosaEvidenceMarkerRecord& Record);

	UFUNCTION(BlueprintCallable, Category = "MUMOSA|Evidence")
	FName GetMarkerId() const { return MarkerId; }

	UFUNCTION(BlueprintCallable, Category = "MUMOSA|Evidence")
	const FMumosaEvidenceMarkerRecord& GetMarkerRecord() const { return MarkerRecord; }

	UFUNCTION(BlueprintCallable, Category = "MUMOSA|Evidence")
	void SetSelected(bool bInSelected);

	UFUNCTION(BlueprintCallable, Category = "MUMOSA|Evidence")
	bool IsSelected() const { return bSelected; }

protected:
	virtual void BeginPlay() override;
	virtual void NotifyActorOnClicked(FKey ButtonPressed) override;

	UPROPERTY(VisibleAnywhere, Category = "MUMOSA|Evidence")
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, Category = "MUMOSA|Evidence")
	TObjectPtr<UStaticMeshComponent> MarkerMesh;

	UPROPERTY(VisibleAnywhere, Category = "MUMOSA|Evidence")
	TObjectPtr<class UTextRenderComponent> LabelText;

	UPROPERTY(VisibleAnywhere, Category = "MUMOSA|Evidence")
	TObjectPtr<class USphereComponent> InteractionVolume;

	UPROPERTY()
	FMumosaEvidenceMarkerRecord MarkerRecord;

	UPROPERTY()
	FName MarkerId;

	UPROPERTY()
	bool bSelected;

	UPROPERTY(EditDefaultsOnly, Category = "MUMOSA|Evidence")
	UMaterialInterface* DefaultMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "MUMOSA|Evidence")
	UMaterialInterface* SelectedMaterial;
};
