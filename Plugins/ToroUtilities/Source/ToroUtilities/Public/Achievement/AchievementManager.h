// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "AchievementTypes.h"
#include "Helpers/ClassGetters.h"
#include "SaveSystem/ToroSaveObjects.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AchievementManager.generated.h"

UCLASS(BlueprintType, DisplayName = "Achievements")
class TOROUTILITIES_API UAchievementManager final : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UAchievementManager() {}
	GAME_INSTANCE_SUBSYSTEM_GETTER(UAchievementManager);

	UFUNCTION(BlueprintCallable, Category = Achievements)
		void Grant(UPARAM(meta = (Categories = "Achievement")) const FGameplayTag InTag, const uint8 Value = 1);

	UFUNCTION(BlueprintCallable, Category = Achievements)
		uint8 GetProgress(UPARAM(meta = (Categories = "Achievement")) const FGameplayTag InTag);

	UFUNCTION(BlueprintCallable, Category = Achievements)
		bool HasAchievement(UPARAM(meta = (Categories = "Achievement")) const FGameplayTag InTag);

	DECLARE_DELEGATE_TwoParams(FAchievementEvent, const FAchievementEntry&, const uint8)
	FAchievementEvent OnAchievement;

private:

	UPROPERTY() TObjectPtr<UAchievementDatabase> Database;
	UPROPERTY() TObjectPtr<UToroGlobalSave> SaveObject;

	UToroGlobalSave* GetSaveObject();
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};
