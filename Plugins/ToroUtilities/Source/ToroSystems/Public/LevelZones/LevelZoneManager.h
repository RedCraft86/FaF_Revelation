// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroSettings.h"
#include "Helpers/GameplayTagMacros.h"
#include "Subsystems/WorldSubsystem.h"
#include "LevelZoneManager.generated.h"

namespace ZoneTags
{
	TOROSYSTEMS_API DECLARE_GAMEPLAY_TAG(Zone)
	TOROSYSTEMS_API CREATE_TAG_VERIFIER(Zone)
}

UCLASS(BlueprintType, DisplayName = "Zone Manager")
class TOROSYSTEMS_API ULevelZoneManager final : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	ULevelZoneManager() {}

	WORLD_SUBSYSTEM_GETTER(ULevelZoneManager);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FZoneActivity, const FGameplayTag&, Character, const FGameplayTag&, Zone);
	UPROPERTY(BlueprintAssignable) FZoneActivity OnEnterZone;
	UPROPERTY(BlueprintAssignable) FZoneActivity OnExitZone;

	UFUNCTION(BlueprintCallable, Category = LevelZones)
		bool IsCharacterInZone(UPARAM(meta = (Categories = "Character")) const FGameplayTag Character,
			UPARAM(meta = (Categories = "Zone")) const FGameplayTag Zone);

	UFUNCTION(BlueprintCallable, Category = LevelZones)
		FGameplayTag GetZoneByCharacter(UPARAM(meta = (Categories = "Character")) const FGameplayTag Character);
	
	UFUNCTION(BlueprintCallable, Category = LevelZones)
		FGameplayTagContainer GetCharactersInZone(UPARAM(meta = (Categories = "Zone")) const FGameplayTag Zone);
	
	void AssignCharToZone(const FGameplayTag& Char, const FGameplayTag& Zone);

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override
	{
		return Super::ShouldCreateSubsystem(Outer) && UToroSettings::Get()->IsOnGameplayMap(Outer);
	}

	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override
	{
		return WorldType == EWorldType::Game || WorldType == EWorldType::PIE ||
			WorldType == EWorldType::GamePreview || WorldType == EWorldType::GameRPC;
	}

private:

	UPROPERTY() TMap<FGameplayTag, FGameplayTag> CharToZone;
};
