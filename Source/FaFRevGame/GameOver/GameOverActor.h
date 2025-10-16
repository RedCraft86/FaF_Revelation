// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "CineCameraActor.h"
#include "Actors/ToroCharacter.h"
#include "GameOverActor.generated.h"

UCLASS(HideCategories = (Replication, Networking, Mobile, Input, RayTracing, Cooking), PrioritizeCategories = (Settings))
class FAFREVGAME_API AGameOverActor final : public ACineCameraActor
{
	GENERATED_BODY()

public:

	AGameOverActor(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = Statics, meta = (WorldContext = "ContextObject"))
		static void InitiateGameOver(const UObject* ContextObject);

	UFUNCTION(BlueprintCallable, Category = GameOver)
		void ShowGameOver();

	UFUNCTION(BlueprintPure, Category = GameOver)
		FGameplayTag GetCharacterID() const { return CharacterID; }

protected:

	UPROPERTY(EditAnywhere, Category = Settings, meta = (Categories = "Character"))
		FGameplayTag CharacterID;

	UPROPERTY(EditAnywhere, Category = Settings)
		FText DisplayName;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (MultiLine = true))
		FText Description;
};
