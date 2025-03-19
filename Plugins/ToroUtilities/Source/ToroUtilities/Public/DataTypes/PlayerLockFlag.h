// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroCore.h"
#include "GameplayTagContainer.h"
#include "PlayerLockFlag.generated.h"

UE_DECLARE_GAMEPLAY_TAG_BASE(PlayerLock)
UE_DECLARE_GAMEPLAY_TAG_CHILD(Cinematic, PlayerLock)

USTRUCT(BlueprintType)
struct TOROUTILITIES_API FPlayerLockFlag
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = PlayerLock, meta = (Categories = "PlayerLock"))
		FGameplayTag LockTag;

	UPROPERTY(EditAnywhere, Category = PlayerLock)
		FName LockKey;

	FPlayerLockFlag();
	FPlayerLockFlag(const FName& InLock);
	FPlayerLockFlag(const FGameplayTag& InLock);
	FORCEINLINE friend bool operator==(const FPlayerLockFlag& A, const FPlayerLockFlag& B) { return A.Get() == B.Get(); }
	FORCEINLINE friend bool operator!=(const FPlayerLockFlag& A, const FPlayerLockFlag& B) { return !(A == B); }
	FORCEINLINE friend FName operator*(const FPlayerLockFlag& InLock) { return InLock.Get(); }
	FORCEINLINE bool IsValidFlag() const { return !Get().IsNone(); }
	FORCEINLINE FString ToString() const { return Get().ToString(); }
	FORCEINLINE FName Get() const;

#if WITH_EDITOR
	FORCEINLINE void ResetTag();
	FORCEINLINE bool HasValidTag() const;
#endif
};

inline void DeduplicateLockFlags(TArray<FPlayerLockFlag>& Flags)
{
	TSet<FName> Existing;
	for (auto It = Flags.CreateIterator(); It; ++It)
	{
		if (FName Flag = **It; Existing.Contains(Flag))
		{
			if (It.GetIndex() == Flags.Num() - 1)
			{
				*It = FPlayerLockFlag();
			}
			else It.RemoveCurrent();
		}
		else Existing.Add(Flag);
	}
}
