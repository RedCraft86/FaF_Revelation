// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Actors/ToroActor.h"
#include "Inventory/InventoryAsset.h"
#include "Interfaces/ExitInterface.h"
#include "Components/ArrowComponent.h"
#include "DataTypes/ToroInterpolator.h"
#include "Framework/ToroCameraManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interaction/InteractionInterface.h"
#include "InspectableActor.generated.h"

UCLASS()
class TOROSYSTEMS_API AInspectableActor final : public AToroActor, public IExitInterface, public IInteractionInterface
{
	GENERATED_BODY()

	friend class UInspectionComponent;

public:

	AInspectableActor();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USpringArmComponent> InspectRoot;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UArrowComponent> SecretAngle;

	UPROPERTY(EditAnywhere, Category = Settings)
		TObjectPtr<UInventoryAsset> Archive;

	/* Secrets are a dot product of Secret Angle to Camera.
	 * Where 1 means directly facing and -1 means not facing at all.
	 * Secret is considered found if inclusively between 1.0 and SecretThreshold.
	 */
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
	virtual FInteractionInfo GetInteractInfo_Implementation(const FHitResult& HitResult) override
	{
		InteractionInfo.bEnabled = HasValidArchive() && InteractionInfo.bEnabled && IsEnabled();
		return InteractionInfo;
	}


private:

	FToroInterpFloat ScaleLerp;
	UPROPERTY() FTimerHandle LagTimer;
	UPROPERTY(Transient) TObjectPtr<UInspectionComponent> Manager;
	UPROPERTY(Transient) TObjectPtr<AToroCameraManager> CamManager;

	void OnEndInspect();
	void HandleRemoveLag();
	float GetSecretDot() const;
	bool SeenSecret() const { return GetSecretDot() >= SecretThreshold; }
	bool HasValidArchive() const { return Archive && Archive->IsValidArchive(); }

	virtual void Tick(float DeltaSeconds) override;
	virtual void OnConstruction(const FTransform& Transform) override;
#if WITH_EDITOR
	virtual bool CanEditChange(const FProperty* InProperty) const override
	{
		const bool bSuper = Super::CanEditChange(InProperty);
		if (InProperty && InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(AInspectableActor, SecretThreshold))
		{
			return bSuper && HasValidArchive();
		}
		return bSuper;
	}
#endif
};
