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

	virtual void GetViewPoint_Implementation(FVector& Location, FVector& Forward, float& Angle) const override {}
	virtual FGameplayTag GetCharacterID_Implementation() const override { return CharacterID; }
	virtual FVector GetLookTarget_Implementation() const override
	{
		return GetActorTransform().TransformPosition({0.0f, 0.0f, 60.0f});
	}

protected:

	UPROPERTY(EditAnywhere, Category = CharGeneral, meta = (DisplayPriority = -2, Categories = "Character"))
		FGameplayTag CharacterID;
};
