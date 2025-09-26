// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Actors/ToroActor.h"
#include "Interfaces/ExitInterface.h"
#include "Inventory/InventoryAsset.h"
#include "DataTypes/ToroInterpolator.h"
#include "Interaction/InteractionInterface.h"
#include "InspectableActor.generated.h"

UCLASS()
class TORORUNTIME_API AInspectableActor final : public AToroActor, public IExitInterface, public IInteractionInterface
{
	GENERATED_BODY()

	friend class UInspectionManager;

public:

	AInspectableActor();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class USpringArmComponent> InspectRoot;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UArrowComponent> SecretAngle;

	UPROPERTY(EditAnywhere, Category = Settings)
		TObjectPtr<UInventoryAsset> Archive;

	/* 1 = Directly looking at SecretAngle, -1 is the opposite. A secret is found if between 1 and this value. */
	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 0.0f, UIMin = 0.0f, ClampMax = 0.9f, UIMax = 0.9f))
		float SecretThreshold;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 0.1f, UIMin = 0.1f, AllowPreserveRatio = true))
		FVector InspectScale;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float ScaleSpeed;

	UPROPERTY(EditAnywhere, Category = Settings, AdvancedDisplay)
		FInteractionInfo InteractionInfo;

	virtual void Exit_Implementation() override;
	virtual void OnBeginInteract_Implementation(AToroPlayerCharacter* Player, const FHitResult& HitResult) override;
	virtual FInteractionInfo GetInteractInfo_Implementation(const FHitResult& HitResult) override;

private:

	FTimerHandle LagTimer;
	FToroInterpVector ScaleInterp;
	TObjectPtr<UInspectionManager> Manager;
	TObjectPtr<APlayerCameraManager> CamManager;

	void OnEndInspect();
	void HandleRemoveLag();
	float GetSecretDot() const;
	bool SeenSecret() const { return GetSecretDot() >= SecretThreshold; }
	bool HasValidArchive() const { return Archive && Archive->IsValidArchive(); }

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnConstruction(const FTransform& Transform) override;
};
