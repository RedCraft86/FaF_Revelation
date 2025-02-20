// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroSaveObject.h"
#include "ClassGetterHelpers.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ToroSaveManager.generated.h"

UCLASS(BlueprintType, DisplayName = "Save Manager")
class TORORUNTIME_API UToroSaveManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UToroSaveManager() {}
	GAME_INSTANCE_SUBSYSTEM_GETTER(UToroSaveManager);

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
