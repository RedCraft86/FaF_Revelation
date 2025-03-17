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

	virtual FGameplayTag GetCharacterID_Implementation() const override { return CharacterID; }

protected:

	UPROPERTY(EditAnywhere, Category = Settings, meta = (Categories = "Character"))
		FGameplayTag CharacterID;
};
