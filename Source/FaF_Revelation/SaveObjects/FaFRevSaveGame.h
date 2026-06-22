// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "SaveSystem/ToroSaveGame.h"
#include "FaFRevSaveGame.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class FAF_REVELATION_API UFaFRevSaveGame : public UToroSaveGame
{
	GENERATED_BODY()
	
public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFlagAddedDelegateBP, const FGameplayTag&, Flag);
	UPROPERTY(BlueprintAssignable, Category = SaveGame, DisplayName = "On Flag Added")
		FOnFlagAddedDelegateBP OnFlagAddedBP;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnFlagAddedDelegate, const FGameplayTag&);
	FOnFlagAddedDelegate OnFlagAdded;

	UFUNCTION(BlueprintPure, Category = SaveGame)
	void AddGameFlag(const FGameplayTag& InFlag)
	{
		if (InFlag.IsValid() && !Flags.Contains(InFlag))
		{
			Flags.Add(InFlag);
			OnFlagAdded.Broadcast(InFlag);
			OnFlagAddedBP.Broadcast(InFlag);
		}
	}

	UFUNCTION(BlueprintPure, Category = SaveGame)
	bool HasGameFlag(const FGameplayTag& InFlag) const
	{
		return InFlag.IsValid() && Flags.Contains(InFlag);
	}

protected:

	UPROPERTY(SaveGame)
		TSet<FGameplayTag> Flags;
};
