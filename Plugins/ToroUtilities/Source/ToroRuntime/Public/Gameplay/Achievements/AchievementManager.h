// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "AchievementDatabase.h"
#include "Framework/ToroGameState.h"
#include "Components/ActorComponent.h"
#include "Helpers/ClassGetterMacros.h"
#include "SaveSystem/ToroSaveManager.h"
#include "AchievementManager.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (Player), meta = (BlueprintSpawnableComponent))
class TORORUNTIME_API UAchievementManager final : public UActorComponent
{
	GENERATED_BODY()

public:

	UAchievementManager() {}
	
	GLOBAL_COMPONENT_GETTER(UAchievementManager, AToroGameState, Achievements)

	UFUNCTION(BlueprintCallable, Category = Statics, meta = (WorldContext = "ContextObject"))
	static UAchievementManager* GetAchievementManager(const UObject* ContextObject)
	{
		return Get(ContextObject);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Achievements)
		void Grant(UPARAM(meta = (Categories = "Achievement")) const FGameplayTag Key, const uint8 Value = 1) const;

	UFUNCTION(BlueprintCallable, Category = Achievements)
		uint8 GetProgress(UPARAM(meta = (Categories = "Achievement")) const FGameplayTag Key) const;
	
	UFUNCTION(BlueprintCallable, Category = Achievements)
		bool HasAchievement(UPARAM(meta = (Categories = "Achievement")) const FGameplayTag Key) const;

	// TODO widget
	DECLARE_MULTICAST_DELEGATE_TwoParams(FAchievementEvent, const FAchievementEntry&, const uint8)
	FAchievementEvent OnAchievement;

private:

	UPROPERTY(Transient)
		TObjectPtr<UAchievementDatabase> Database;

	TObjectPtr<UToroSaveManager> SaveManager;

	virtual void BeginPlay() override;
};
