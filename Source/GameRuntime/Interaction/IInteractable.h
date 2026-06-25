// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "IInteractable.generated.h"

class AToroCharacter;

UINTERFACE()
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

class IInteractable
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, Category = Interaction)
		void BeginInteract(AToroCharacter* Instigator);
	virtual void BeginInteract_Implementation(AToroCharacter* Instigator) = 0;

	UFUNCTION(BlueprintNativeEvent, Category = Interaction)
		void EndInteract(AToroCharacter* Instigator);
	virtual void EndInteract_Implementation(AToroCharacter* Instigator) {}

	UFUNCTION(BlueprintNativeEvent, Category = Interaction)
		bool GetInteractionInfo(FText& Label);
	virtual bool GetInteractionInfo_Implementation(FText& Label) { return false; }

	static bool ImplementedBy(const UObject* Target) 
	{ 
		return IsValid(Target) && Target->Implements<UInteractable>(); 
	}

	static void BeginInteract(UObject* Target, AToroCharacter* Instigator)
	{
		if (ImplementedBy(Target))
		{
			Execute_BeginInteract(Target, Instigator);
		}
	}

	static void EndInteract(UObject* Target, AToroCharacter* Instigator)
	{
		if (ImplementedBy(Target))
		{
			Execute_EndInteract(Target, Instigator);
		}
	}

	static bool GetInteractionInfo(UObject* Target, FText& Label)
	{
		return ImplementedBy(Target) && Execute_GetInteractionInfo(Target, Label);
	}
};
