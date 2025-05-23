// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "Helpers/ClassGetters.h"
#include "GameFramework/GameStateBase.h"
#include "ToroGameState.generated.h"

UCLASS()
class TOROUTILITIES_API AToroGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	AToroGameState();

	GAMEPLAY_CLASS_GETTER(AToroGameState, GetGameState)

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

	UFUNCTION(BlueprintCallable, Category = Game)
		bool IsCharacterInZone(UPARAM(meta = (Categories = "Character")) const FGameplayTag Character,
			UPARAM(meta = (Categories = "Zone")) const FGameplayTag Zone);

	UFUNCTION(BlueprintCallable, Category = Game)
		FGameplayTag GetCharacterZone(UPARAM(meta = (Categories = "Character")) const FGameplayTag Character);

	UFUNCTION(BlueprintCallable, Category = Game)
		FGameplayTagContainer GetAllCharactersInZone(UPARAM(meta = (Categories = "Zone")) const FGameplayTag Zone);
	
	void AssignCharacterToZone(const FGameplayTag Character, const FGameplayTag Zone)
	{
		if (Character.IsValid()) CharacterToZone.Add(Character, Zone);
	}
	
protected:

	UPROPERTY() TMap<FGameplayTag, FGameplayTag> CharacterToZone;
};
