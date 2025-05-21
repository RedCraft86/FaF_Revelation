// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "CharacterInterface.generated.h"

UINTERFACE()
class UCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

class TOROCORE_API ICharacterInterface
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintNativeEvent, Category = Character)
		FGameplayTag GetCharacterID() const;
	virtual FGameplayTag GetCharacterID_Implementation() const { return {}; }

	UFUNCTION(BlueprintNativeEvent, Category = Character)
		void GetViewPoint(FVector& Location, FVector& Forward, float& Angle);
	virtual void GetViewPoint_Implementation(FVector& Location, FVector& Forward, float& Angle) {}

	// Location to use when OTHER characters need to look at THIS character.
	UFUNCTION(BlueprintNativeEvent, Category = Character)
		FVector GetLookTarget();
	virtual FVector GetLookTarget_Implementation() { return FVector::ZeroVector; }

	static bool ImplementedBy(const UObject* Target) 
	{ 
		return IsValid(Target) ? Target->Implements<UCharacterInterface>() : false; 
	}

	static FGameplayTag GetCharacterID(const UObject* Target)
	{
		if (ImplementedBy(Target))
		{
			return ICharacterInterface::Execute_GetCharacterID(Target);
		}

		return {};
	}

	static void GetViewPoint(UObject* Target, FVector& Location, FVector& Forward, float& FOV)
	{
		if (ImplementedBy(Target))
		{
			ICharacterInterface::Execute_GetViewPoint(Target, Location, Forward, FOV);
		}
	}

	static FVector GetLookTarget(UObject* Target)
	{
		if (ImplementedBy(Target))
		{
			return ICharacterInterface::Execute_GetLookTarget(Target);
		}

		AActor* AsActor = Cast<AActor>(Target);
		return AsActor ? AsActor->GetActorLocation() : FVector::ZeroVector;
	}
};