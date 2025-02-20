// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "AchievementTypes.h"
#include "ClassGetterHelpers.h"
#include "SaveSystem/BaseSaveObjects.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AchievementManager.generated.h"

UCLASS(BlueprintType, DisplayName = "Achievements")
class TORORUNTIME_API UAchievementManager final : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UAchievementManager() {}
	GAME_INSTANCE_SUBSYSTEM_GETTER(UAchievementManager);

	UFUNCTION(BlueprintCallable, Category = Achievements)
		void Grant(UPARAM(meta = (Categories = "Achievements")) const FGameplayTag InTag, const uint8 Value = 1);

	UFUNCTION(BlueprintCallable, Category = Achievements)
		uint8 GetProgress(UPARAM(meta = (Categories = "Achievements")) const FGameplayTag InTag);
	
	UFUNCTION(BlueprintCallable, Category = Achievements)
		bool HasAchievement(UPARAM(meta = (Categories = "Achievements")) const FGameplayTag InTag);
	
private:

	UPROPERTY() TObjectPtr<UAchievementDatabase> Database;
	UPROPERTY() TObjectPtr<UGlobalSaveObjectBase> SaveObject;

	UGlobalSaveObjectBase* GetSaveObject();
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};
