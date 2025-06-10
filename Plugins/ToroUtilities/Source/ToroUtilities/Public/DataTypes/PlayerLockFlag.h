// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Helpers/GameplayTagMacros.h"
#include "PlayerLockFlag.generated.h"

namespace PlayerLockTags
{
	TOROUTILITIES_API DECLARE_GAMEPLAY_TAG(PlayerLock)
    TOROUTILITIES_API DECLARE_GAMEPLAY_TAG(MainMenu)
    TOROUTILITIES_API DECLARE_GAMEPLAY_TAG(Startup)
    TOROUTILITIES_API DECLARE_GAMEPLAY_TAG(Loading)
    TOROUTILITIES_API DECLARE_GAMEPLAY_TAG(Cinematic)
	
	TOROUTILITIES_API CREATE_TAG_VERIFIER(PlayerLock)
}

USTRUCT(BlueprintType)
struct TOROUTILITIES_API FPlayerLockFlag
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = PlayerLock, meta = (Categories = "PlayerLock"))
		FGameplayTag LockTag;

	UPROPERTY(EditAnywhere, Category = PlayerLock)
		FName LockKey;

	FPlayerLockFlag(): LockTag(PlayerLockTags::TAG_PlayerLock), LockKey(NAME_None) {}
	FPlayerLockFlag(const FName& InLock): LockTag(PlayerLockTags::TAG_PlayerLock), LockKey(InLock) {}
	FPlayerLockFlag(const FString& InLock): LockTag(PlayerLockTags::TAG_PlayerLock), LockKey(*InLock) {}
	FPlayerLockFlag(const FGameplayTag& InLock): LockTag(InLock), LockKey(NAME_None) {}

	FORCEINLINE friend uint32 GetTypeHash(const FPlayerLockFlag& InFlag) { return GetTypeHash(InFlag.ToString()); }
	FORCEINLINE friend bool operator==(const FPlayerLockFlag& A, const FPlayerLockFlag& B) { return *A == *B; }
	FORCEINLINE friend bool operator!=(const FPlayerLockFlag& A, const FPlayerLockFlag& B) { return !(A == B); }
	FORCEINLINE friend FName operator*(const FPlayerLockFlag& InFlag) { return InFlag.Get(); }
	
	FORCEINLINE FName Get() const { return PlayerLockTags::IsValidTag(LockTag) ? LockTag.GetTagName() : LockKey; }
	FORCEINLINE bool IsValidFlag() const { return !Get().IsNone(); }
	FORCEINLINE FString ToString() const { return Get().ToString(); }

#if WITH_EDITOR
	FORCEINLINE void ResetTag() { LockTag = PlayerLockTags::TAG_PlayerLock; }
	FORCEINLINE bool HasValidTag() const { return PlayerLockTags::IsValidTag(LockTag); }
#endif

	static bool DedupeFlags(TArray<FPlayerLockFlag>& Flags);
};