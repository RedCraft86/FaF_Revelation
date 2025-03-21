// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroSaveObject.h"
#include "ClassGetterHelpers.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ToroSaveManager.generated.h"

class UToroSaveObject;

UCLASS(BlueprintType, DisplayName = "Save Manager")
class TOROUTILITIES_API UToroSaveManager final : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UToroSaveManager() : ActiveGameSave(Tag_Save_Game) {}
	GAME_INSTANCE_SUBSYSTEM_GETTER(UToroSaveManager);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveSystem)
		FGameplayTag ActiveGameSave;

	UFUNCTION(BlueprintPure, Category = SaveSystem)
		UToroSaveObject* GetSaveObject(UPARAM(meta = (Categories = "Save")) const FGameplayTag InTag);

	template <typename T = UToroSaveObject>
	T* GetSaveObject(const FGameplayTag& InTag) { return Cast<T>(GetSaveObject(InTag)); }

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnSaveObjectIO, const UToroSaveObject*)
	FOnSaveObjectIO OnSaveObjectIO;

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
