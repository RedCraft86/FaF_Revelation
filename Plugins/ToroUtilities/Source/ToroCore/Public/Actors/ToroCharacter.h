// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameFramework/Character.h"
#include "Helpers/GameplayTagHelpers.h"
#include "ToroCharacter.generated.h"

DECLARE_GAMEPLAY_TAG_BASE(Character)

UCLASS(Abstract)
class TOROCORE_API AToroCharacter : public ACharacter
{
	GENERATED_BODY()

#if WITH_EDITOR
	friend class FToroActorDetails;
#endif
public:

	AToroCharacter();

	UFUNCTION(BlueprintPure, Category = Game)
		FGameplayTag GetCharacterID() const { return CharacterID; }

	UFUNCTION(BlueprintPure, Category = Game)
		virtual FVector GetLookTarget() const;

	UFUNCTION(BlueprintPure, Category = Game)
		virtual void GetViewPoint(FVector& Location, FVector& Forward, float& Angle) const {}

	UFUNCTION(BlueprintCallable, Category = Game)
		virtual void Teleport(const FVector& InLocation, const FRotator& InRotation);

protected:

	UPROPERTY(EditAnywhere, Category = CharGeneral, meta = (DisplayPriority = -2, Categories = "Character"))
		FGameplayTag CharacterID;
};
