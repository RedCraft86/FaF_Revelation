// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "GameFlagManager.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class UGameFlagManager final : public UActorComponent
{
	GENERATED_BODY()

public:

	UGameFlagManager()
	{
		PrimaryComponentTick.bCanEverTick = false;
	}

	UFUNCTION(BlueprintCallable, Category = GameFlags)
	void UnlockFlag(const FGameplayTag& InFlag)
	{
		if (InFlag.IsValid())
		{
			UnlockRawFlag(InFlag.GetTagName());
		}
	}

	UFUNCTION(BlueprintCallable, Category = GameFlags)
	void UnlockRawFlag(const FName& InFlag)
	{
		if (!InFlag.IsNone() && !IsRawFlagUnlocked(InFlag))
		{
			Flags.Add(InFlag);
			OnFlagUnlocked.Broadcast(InFlag);
			OnFlagUnlockedBP.Broadcast(InFlag);
		}
	}

	UFUNCTION(BlueprintPure, Category = GameFlags)
	bool IsFlagUnlocked(const FGameplayTag& InFlag) const
	{
		return InFlag.IsValid() && IsRawFlagUnlocked(InFlag.GetTagName());
	}

	UFUNCTION(BlueprintPure, Category = GameFlags)
	bool IsRawFlagUnlocked(const FName& InFlag) const
	{
		return !InFlag.IsNone() && Flags.Contains(InFlag);
	}

	const TSet<FName>& GetFlagList() const { return Flags; }
	void SetFlagList(const TSet<FName>& FlagList) { Flags = FlagList; }

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFlagUnlockedDelegateBP, const FName&, Flag);
	UPROPERTY(BlueprintAssignable, Category	= GameFlags, DisplayName = "On Flag Unlocked")
		FOnFlagUnlockedDelegateBP OnFlagUnlockedBP;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnFlagUnlockedDelegate, const FName&);
	FOnFlagUnlockedDelegate OnFlagUnlocked;

private:

	UPROPERTY(Transient)
		TSet<FName> Flags;
};
