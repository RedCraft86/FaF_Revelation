// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "FRCharacter.h"
#include "Components/VisionConeComponent.h"
#include "FREnemyBase.generated.h"

UENUM(BlueprintType, DisplayName = "Enemy AI Mode")
enum class EEnemyState : uint8
{
	None,
	Roam,
	Alert,
	Chase,
	Search
};
ENUM_RANGE_BY_FIRST_AND_LAST(EEnemyState, EEnemyState::None, EEnemyState::Search);

UCLASS(Abstract, DisplayName = "Enemy Base")
class FAF_REV_API AFREnemyBase : public AFRCharacter
{
	GENERATED_BODY()

public:

	AFREnemyBase();

	UPROPERTY(EditAnywhere, Category = "Settings")
		bool bStartRoaming;

	UFUNCTION(BlueprintImplementableEvent)
		FName GetHeadBoneName();

	UFUNCTION(BlueprintCallable, Category = "EnemyAI")
		bool JumpscarePlayer();
	
	UFUNCTION(BlueprintCallable, Category = "EnemyAI")
		void SetEnemyState(const EEnemyState InNewState);

	UFUNCTION(BlueprintPure, Category = "EnemyAI")
		EEnemyState GetEnemyState() const { return EnemyState; }

protected:
	
	UPROPERTY() EEnemyState EnemyState;

	virtual void BeginPlay() override;
};


UCLASS(Abstract, DisplayName = "Enemy Base (Sighted)")
class FAF_REV_API AFRSightedEnemyBase : public AFREnemyBase
{
	GENERATED_BODY()

public:

	AFRSightedEnemyBase();

	UPROPERTY(EditAnywhere, Category = "Settings")
		TObjectPtr<UVisionConeComponent> VisionCone;

protected:
#if WITH_EDITOR
	FName VisionAttachedBone;
#endif
	virtual void OnConstruction(const FTransform& Transform) override;
};