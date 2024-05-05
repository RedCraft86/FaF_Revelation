﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

class AFRPlayerBase;

#define DEFAULT_INTERACT_LABEL NSLOCTEXT("Interaction", "GenericLabel", "Interact")

USTRUCT(BlueprintType)
struct FInteractionInfo
{
	GENERATED_BODY()

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Data")
		FText Label;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Data")
		UTexture2D* Icon;
	
	FInteractionInfo() : Label(DEFAULT_INTERACT_LABEL), Icon(nullptr) {}
};

UINTERFACE()
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class FAF_REV_API IInteractionInterface
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction", DisplayName = "Begin Interact")
		void OnBeginInteract(AFRPlayerBase* Player, const FHitResult& HitResult);
	virtual void OnBeginInteract_Implementation(AFRPlayerBase* Player, const FHitResult& HitResult) {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction", DisplayName = "End Interact")
		void OnEndInteract(AFRPlayerBase* Player);
	virtual void OnEndInteract_Implementation(AFRPlayerBase* Player) {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction", DisplayName = "Begin Pawn Interact")
		void OnBeginPawnInteract(APawn* Pawn, const FHitResult& HitResult);
	virtual void OnBeginPawnInteract_Implementation(APawn* Pawn, const FHitResult& HitResult) {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		bool GetInteractionInfo(FInteractionInfo& Info);
	virtual bool GetInteractionInfo_Implementation(FInteractionInfo& Info)
	{
		Info = {};
		return false;
	}
};

namespace IInteract
{
	static bool ImplementedBy(const UObject* Target) 
	{ 
		return IsValid(Target) ? Target->Implements<UInteractionInterface>() : false; 
	} 
	
	static void BeginInteract(UObject* Target, AFRPlayerBase* Player, const FHitResult& HitResult)
	{
		if (ImplementedBy(Target))
		{
			IInteractionInterface::Execute_OnBeginInteract(Target, Player, HitResult);
		}
	}
	
	static void EndInteract(UObject* Target, AFRPlayerBase* Player)
	{
		if (ImplementedBy(Target))
		{
			IInteractionInterface::Execute_OnEndInteract(Target, Player);
		}
	}

	static void BeginPawnInteract(UObject* Target, APawn* Pawn, const FHitResult& HitResult)
	{
		if (ImplementedBy(Target))
		{
			IInteractionInterface::Execute_OnBeginPawnInteract(Target, Pawn, HitResult);
		}
	}

	static bool GetInteractionInfo(UObject* Target, FInteractionInfo& Info)
	{
		if (ImplementedBy(Target))
		{
			return IInteractionInterface::Execute_GetInteractionInfo(Target, Info);
		}

		Info = {};
		return false;
	}
}
