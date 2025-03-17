// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "CharacterInterface.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "ToroCharacter.generated.h"

UCLASS(Abstract)
class TOROCORE_API AToroCharacter : public ACharacter, public ICharacterInterface
{
	GENERATED_BODY()

public:

	AToroCharacter();

	UFUNCTION(BlueprintCallable, Category = Game)
		virtual void Teleport(const FVector& InLocation, const FRotator& InRotation);

	UFUNCTION(BlueprintPure, Category = Game)
		FGameplayTag GetCharacterID() const { return CharacterID; }

protected:

	UPROPERTY(EditAnywhere, Category = Settings, meta = (Categories = "Characters"))
		FGameplayTag CharacterID;
};
