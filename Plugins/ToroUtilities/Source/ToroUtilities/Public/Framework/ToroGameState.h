// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "Helpers/ClassGetters.h"
#include "GameFramework/GameStateBase.h"
#include "Helpers/GameplayTagHelpers.h"
#include "ToroGameState.generated.h"

DECLARE_GAMEPLAY_TAG_BASE(Zone);

UCLASS()
class TOROUTILITIES_API AToroGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	AToroGameState();

	GAMEPLAY_CLASS_GETTER(AToroGameState, GetGameState)

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

	UFUNCTION(BlueprintPure, Category = Game)
		bool IsCharacterInZone(UPARAM(meta = (Categories = "Character")) const FGameplayTag Character,
			UPARAM(meta = (Categories = "Zone")) const FGameplayTag Zone) const;

	UFUNCTION(BlueprintPure, Category = Game)
		FGameplayTag GetCharacterZone(UPARAM(meta = (Categories = "Character")) const FGameplayTag Character) const;

	UFUNCTION(BlueprintPure, Category = Game)
		FGameplayTagContainer GetAllCharactersInZone(UPARAM(meta = (Categories = "Zone")) const FGameplayTag Zone);
	
	void AssignCharacterToZone(const FGameplayTag Character, const FGameplayTag Zone)
	{
		if (Character.IsValid() && VerifyZoneTag(Zone)) CharacterToZone.Add(Character, Zone);
	}
	
protected:

	UPROPERTY() TMap<FGameplayTag, FGameplayTag> CharacterToZone;
};
