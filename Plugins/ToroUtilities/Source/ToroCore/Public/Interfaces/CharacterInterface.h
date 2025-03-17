// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
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
		void GetViewPoint(FVector& Location, FVector& Forward, float& Angle);
	virtual void GetViewPoint_Implementation(FVector& Location, FVector& Forward, float& Angle) {}

	UFUNCTION(BlueprintNativeEvent, Category = Character)
		bool GetLookTarget(FVector& Location);
	virtual bool GetLookTarget_Implementation(FVector& Location) { return false; }

	static bool ImplementedBy(const UObject* Target) 
	{ 
		return IsValid(Target) ? Target->Implements<UCharacterInterface>() : false; 
	}

	static void GetViewPoint(UObject* Target, FVector& Location, FVector& Forward, float& FOV)
	{
		if (ImplementedBy(Target))
		{
			ICharacterInterface::Execute_GetViewPoint(Target, Location, Forward, FOV);
		}
	}

	static bool GetLookTarget(UObject* Target, FVector& Location)
	{
		if (ImplementedBy(Target))
		{
			return ICharacterInterface::Execute_GetLookTarget(Target, Location);
		}

		return false;
	}
};
