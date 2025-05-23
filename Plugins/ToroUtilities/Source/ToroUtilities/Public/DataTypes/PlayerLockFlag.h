// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Helpers/GameplayTagHelpers.h"
#include "PlayerLockFlag.generated.h"

DECLARE_GAMEPLAY_TAG_BASE(PlayerLock)
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

	FPlayerLockFlag(): LockTag(Tag_PlayerLock), LockKey(NAME_None) {}
	FPlayerLockFlag(const FName& InLock): LockTag(Tag_PlayerLock), LockKey(InLock) {}
	FPlayerLockFlag(const FString& InLock): LockTag(Tag_PlayerLock), LockKey(*InLock) {}
	FPlayerLockFlag(const FGameplayTag& InLock): LockTag(InLock), LockKey(NAME_None) {}

	FORCEINLINE friend uint32 GetTypeHash(const FPlayerLockFlag& InFlag) { return GetTypeHash(InFlag.ToString()); }
	FORCEINLINE friend bool operator==(const FPlayerLockFlag& A, const FPlayerLockFlag& B) { return A.Get() == B.Get(); }
	FORCEINLINE friend bool operator!=(const FPlayerLockFlag& A, const FPlayerLockFlag& B) { return !(A == B); }
	FORCEINLINE friend FName operator*(const FPlayerLockFlag& InFlag) { return InFlag.Get(); }
	
	FORCEINLINE bool IsValidFlag() const { return !Get().IsNone(); }
	FORCEINLINE FString ToString() const { return Get().ToString(); }
	FORCEINLINE FName Get() const { return VerifyPlayerLockTag(LockTag) ? LockTag.GetTagName() : LockKey; }

#if WITH_EDITOR
	FORCEINLINE void ResetTag() { LockTag = Tag_PlayerLock; }
	FORCEINLINE bool HasValidTag() const { return VerifyPlayerLockTag(LockTag); }
#endif

	static void DedupeFlags(TArray<FPlayerLockFlag>& Flags);
};
