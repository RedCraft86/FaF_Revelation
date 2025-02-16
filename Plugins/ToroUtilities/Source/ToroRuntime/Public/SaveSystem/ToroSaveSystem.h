﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroSaveObject.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ToroSaveSystem.generated.h"

UCLASS(BlueprintType)
class TORORUNTIME_API UToroSaveSystem final : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UToroSaveSystem() {}

	UFUNCTION(BlueprintPure, Category = SaveSystem)
		UToroSaveObject* GetSaveObject(UPARAM(meta = (Categories = "Saves")) const FGameplayTag InTag);

	template <typename T = UToroSaveObject>
	T* GetSaveObject(const FGameplayTag& InTag)
	{
		return Cast<T>(GetSaveObject(InTag));
	}

private:
	
	UPROPERTY(Transient) TMap<FGameplayTag, TObjectPtr<UToroSaveObject>> SaveObjects;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
};
