// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "AchievementDatabase.h"
#include "Framework/ToroGameState.h"
#include "Components/ActorComponent.h"
#include "Helpers/ClassGetterMacros.h"
#include "AchievementManager.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (Player), meta = (BlueprintSpawnableComponent))
class TORORUNTIME_API UAchievementManager final : public UActorComponent
{
	GENERATED_BODY()

public:

	UAchievementManager() {}
	
	GLOBAL_COMPONENT_GETTER(UAchievementManager, AToroGameState, Achievements)

	UFUNCTION(BlueprintCallable, Category = Achievements)
		void Grant(UPARAM(meta = (Categories = "Achievement")) const FGameplayTag Key, const uint8 Value = 1);

	UFUNCTION(BlueprintCallable, Category = Achievements)
		uint8 GetProgress(UPARAM(meta = (Categories = "Achievement")) const FGameplayTag Key);
	
	UFUNCTION(BlueprintCallable, Category = Achievements)
		bool HasAchievement(UPARAM(meta = (Categories = "Achievement")) const FGameplayTag Key);

	// TODO widget
	DECLARE_DELEGATE_TwoParams(FAchievementEvent, const FAchievementEntry&, const uint8)
	FAchievementEvent OnAchievement;

private:

	UPROPERTY(Transient)
		TObjectPtr<UAchievementDatabase> Database;

	virtual void BeginPlay() override;
};
