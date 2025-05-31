// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Helpers/GameplayTagHelpers.h"
#include "PlayerLockFlag.generated.h"

DECLARE_GAMEPLAY_TAG_BASE(PlayerLock)
DECLARE_GAMEPLAY_TAG_CHILD(PlayerLock, Loading)
DECLARE_GAMEPLAY_TAG_CHILD(PlayerLock, Cinematic)
DECLARE_GAMEPLAY_TAG_CHILD(PlayerLock, Dialogue)
DECLARE_GAMEPLAY_TAG_CHILD(PlayerLock, Inventory)

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
	FPlayerLockFlag(const FString& InLock);
	FPlayerLockFlag(const FGameplayTag& InLock);

	FORCEINLINE friend uint32 GetTypeHash(const FPlayerLockFlag& InFlag) { return GetTypeHash(InFlag.ToString()); }
	FORCEINLINE friend bool operator==(const FPlayerLockFlag& A, const FPlayerLockFlag& B) { return A.Get() == B.Get(); }
	FORCEINLINE friend bool operator!=(const FPlayerLockFlag& A, const FPlayerLockFlag& B) { return !(A == B); }
	FORCEINLINE friend FName operator*(const FPlayerLockFlag& InFlag) { return InFlag.Get(); }
	
	FORCEINLINE FName Get() const;
	FORCEINLINE bool IsValidFlag() const;
	FORCEINLINE FString ToString() const;

#if WITH_EDITOR
	FORCEINLINE void ResetTag();
	FORCEINLINE bool HasValidTag() const;
#endif

	static bool DedupeFlags(TArray<FPlayerLockFlag>& Flags);
};
