// Copyright (C) RedCraft86. All Rights Reserved.

#include "PlayerActions.h"
#include "Player/PlayerCharacter.h"

void FWTPlayer_LockTag::RunAction()
{
	if (APlayerCharacter* Player = APlayerCharacter::Get<APlayerCharacter>(GetWorldContext()))
	{
		for (const FGameplayTag& Tag : AddTags)
		{
			Player->AddLockTag(Tag);
		}
		for (const FGameplayTag& Tag : RemoveTags)
		{
			Player->RemoveLockTag(Tag);
		}
	}
}

void FWTPlayer_LockOn::RunAction()
{
	if (AActor* TargetPtr = Target.LoadSynchronous())
	{
		if (APlayerCharacter* Player = APlayerCharacter::Get<APlayerCharacter>(GetWorldContext()))
		{
			Player->LockOnSpeed = LockOnSpeed;
			Player->SetLockOnTarget(TargetPtr);
		}
	}
}

void FWTPlayer_MoveSpeed::RunAction()
{
	if (ModifierKey.IsNone() || Multiplier < 0.1f) return;
	if (APlayerCharacter* Player = APlayerCharacter::Get<APlayerCharacter>(GetWorldContext()))
	{
		bRemove ? Player->RemoveMovementSpeedMod(ModifierKey) : Player->AddMovementSpeedMod(ModifierKey, Multiplier);
	}
}
