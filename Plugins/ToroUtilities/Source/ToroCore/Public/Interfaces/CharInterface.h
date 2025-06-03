// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Helpers/GameplayTagHelpers.h"
#include "CharInterface.generated.h"

DECLARE_GAMEPLAY_TAG_BASE(Character)

UINTERFACE()
class UCharInterface : public UInterface
{
	GENERATED_BODY()
};

class TOROCORE_API ICharInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, Category = Character)
		FGameplayTag GetCharacterID() const;
	virtual FGameplayTag GetCharacterID_Implementation() const { return {}; }

	UFUNCTION(BlueprintNativeEvent, Category = Character)
		bool GetLookTarget(FVector& Location) const;
	virtual bool GetLookTarget_Implementation(FVector& Location) const { return false; }

	UFUNCTION(BlueprintNativeEvent, Category = Character)
		void GetViewPoint(FVector& Location, FVector& Forward, float& Angle) const;
	virtual void GetViewPoint_Implementation(FVector& Location, FVector& Forward, float& Angle) const {}

	static bool ImplementedBy(const UObject* Target) 
	{ 
		return IsValid(Target) ? Target->Implements<UCharInterface>() : false; 
	}

	static FGameplayTag GetCharacterID(const UObject* Target)
	{
		if (ImplementedBy(Target))
		{
			return ICharInterface::Execute_GetCharacterID(Target);
		}

		return {};
	}

	static bool GetLookTarget(const UObject* Target, FVector& Location)
	{
		if (ImplementedBy(Target))
		{
			return ICharInterface::Execute_GetLookTarget(Target, Location);
		}

		return false;
	}

	static void GetViewPoint(const UObject* Target, FVector& Location, FVector& Forward, float& FOV)
	{
		if (ImplementedBy(Target))
		{
			ICharInterface::Execute_GetViewPoint(Target, Location, Forward, FOV);
		}
	}
};