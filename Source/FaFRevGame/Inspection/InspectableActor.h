// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Actors/ToroActor.h"
#include "Player/GamePlayerChar.h"
#include "Inventory/InventoryItemData.h"
#include "Interaction/InteractionInterface.h"
#include "InspectableActor.generated.h"

UCLASS()
class FAFREVGAME_API AInspectableActor final : public AToroActor, public IExitInterface, public IInteractionInterface
{
	GENERATED_BODY()

public:

	AInspectableActor();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USpringArmComponent> InspectRoot;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UArrowComponent> SecretAngle;

	UPROPERTY(EditAnywhere, Category = Settings)
		TObjectPtr<UItemData> Archive;

	UPROPERTY(EditAnywhere, Category = Settings)
		float MinSecretDotAngle;

	UPROPERTY(EditAnywhere, Category = Settings)
		FVector2D TurningSpeed;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 0.1f, UIMin = 0.1f, AllowPreserveRatio = true))
		FVector InspectScale;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float ScaleSpeed;

	UPROPERTY(EditAnywhere, Category = Settings, AdvancedDisplay)
		FInteractionInfo InteractionInfo;

	virtual void Exit_Implementation() override;
	virtual void OnBeginInteract_Implementation(AGamePlayerChar* Player, const FHitResult& HitResult) override;
	virtual FInteractionInfo GetInteractionInfo_Implementation(const FHitResult& HitResult) override
	{
		InteractionInfo.bEnabled = Archive && InteractionInfo.bEnabled && IsEnabled();
		return InteractionInfo;
	}

private:

	bool bArchived;
	bool bSecretKnown;
	FTimerHandle LagTimer;
	FToroInterpFloat ScaleLerp;
	TObjectPtr<AGamePlayerChar> PlayerChar;
	TObjectPtr<class UInspectionWidget> Widget;

	void HandleRemoveLag();
	UInspectionWidget* GetWidget();
	float GetSecretDotAngle() const;
	bool HasValidArchive() const { return Archive && Archive->ItemType == EItemType::Archive; }

	virtual void Tick(float DeltaSeconds) override;
	virtual void OnConstruction(const FTransform& Transform) override;
#if WITH_EDITOR
	virtual bool CanEditChange(const FProperty* InProperty) const override
	{
		const bool bSuper = Super::CanEditChange(InProperty);
		if (InProperty && InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(AInspectableActor, MinSecretDotAngle))
		{
			return bSuper && Archive ? Archive->ItemType == EItemType::Archive : false;
		}
		return bSuper;
	}
#endif
};
