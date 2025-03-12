// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ClassGetterHelpers.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ToroSaveManager.generated.h"

class UToroSaveObject;

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

namespace ToroSave
{
	inline UToroSaveManager* GetManager(const UObject* WorldContext)
	{
		return UToroSaveManager::Get(WorldContext);
	}

	template <typename T = UToroSaveObject>
	T* GetObject(const UObject* WorldContext, const FGameplayTag& InTag)
	{
		if (UToroSaveManager* SaveManager = GetManager(WorldContext))
		{
			return SaveManager->GetSaveObject<T>(InTag);
		}
		return nullptr;
	}
}
