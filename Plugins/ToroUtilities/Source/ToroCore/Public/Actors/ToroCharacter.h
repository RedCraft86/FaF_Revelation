// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameFramework/Character.h"
#include "Helpers/GameplayTagMacros.h"
#include "Interfaces/CharInterface.h"
#include "ToroCharacter.generated.h"

namespace CharacterTags
{
	TOROCORE_API DECLARE_GAMEPLAY_TAG(Character)
	TOROCORE_API DECLARE_GAMEPLAY_TAG(Player)

	TOROCORE_API CREATE_TAG_VERIFIER(Character)
}

UCLASS(Abstract)
class TOROCORE_API AToroCharacter : public ACharacter, public ICharInterface
{
	GENERATED_BODY()

public:

	AToroCharacter() {}

	UFUNCTION(BlueprintCallable, Category = Game)
		virtual void Teleport(const FVector& InLocation, const FRotator& InRotation);

	virtual FGameplayTag GetCharacterID_Implementation() const override { return CharacterID; }

protected:

	UPROPERTY(EditAnywhere, Category = Settings, meta = (Categories = "Character"))
		FGameplayTag CharacterID;
};
