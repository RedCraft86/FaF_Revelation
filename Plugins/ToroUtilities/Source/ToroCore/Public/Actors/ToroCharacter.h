// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameFramework/Character.h"
#include "Helpers/GameplayTagHelpers.h"
#include "Interfaces/CharInterface.h"
#include "ToroCharacter.generated.h"

UCLASS(Abstract)
class TOROCORE_API AToroCharacter : public ACharacter, public ICharInterface
{
	GENERATED_BODY()

#if WITH_EDITOR
	friend class FToroActorDetails;
#endif
public:

	AToroCharacter();

	UFUNCTION(BlueprintCallable, Category = Game)
		virtual void Teleport(const FVector& InLocation, const FRotator& InRotation);

	virtual FGameplayTag GetCharacterID_Implementation() const override { return CharacterID; }

protected:

	UPROPERTY(EditAnywhere, Category = CharGeneral, meta = (DisplayPriority = -2, Categories = "Character"))
		FGameplayTag CharacterID;
};
