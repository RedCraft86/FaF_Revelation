// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "WorldActions/WorldActionBase.h"
#include "PlayerActions.generated.h"

USTRUCT(BlueprintInternalUseOnly, DisplayName = "Player", meta = (Hidden))
struct TORORUNTIME_API FWATask_Player : public FWorldActionBase
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType, DisplayName = "Lock Tag")
struct TORORUNTIME_API FWTPlayer_LockTag final : public FWATask_Player
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Action, meta = (Categories = "PlayerLock"))
		FGameplayTagContainer AddTags;

	UPROPERTY(EditAnywhere, Category = Action, meta = (Categories = "PlayerLock"))
		FGameplayTagContainer RemoveTags;

	FWTPlayer_LockTag() {}

protected:

	virtual void RunAction() override;
};

USTRUCT(BlueprintType, DisplayName = "Lock On")
struct TORORUNTIME_API FWTPlayer_LockOn final : public FWATask_Player
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Action)
		float LockOnSpeed;

	UPROPERTY(EditAnywhere, Category = Action, meta = (MustImplement = "/Script/ToroCore.CharInterface"))
		TSoftObjectPtr<AActor> Target;

	FWTPlayer_LockOn(): LockOnSpeed(false) {}

protected:

	virtual void RunAction() override;
};

USTRUCT(BlueprintType, DisplayName = "Move Speed")
struct TORORUNTIME_API FWTPlayer_MoveSpeed final : public FWATask_Player
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Action)
		bool bRemove;

	UPROPERTY(EditAnywhere, Category = Action)
		FName ModifierKey;

	UPROPERTY(EditAnywhere, Category = Action, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float Multiplier;

	FWTPlayer_MoveSpeed(): bRemove(false), Multiplier(1.0f) {}

protected:

	virtual void RunAction() override;
};
