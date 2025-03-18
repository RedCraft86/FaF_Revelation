// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

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

	UToroSaveManager() {}
	GAME_INSTANCE_SUBSYSTEM_GETTER(UToroSaveManager);

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
