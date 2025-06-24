// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

class AToroPlayerCharacter;

USTRUCT(BlueprintType)
struct TOROSYSTEMS_API FInteractionInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		bool bEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (MultiLine = true))
		FText Label;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		TObjectPtr<UTexture2D> Icon;
	
	FInteractionInfo(): bEnabled(true), Label(INVTEXT("Interact")), Icon(nullptr) {}
	static FInteractionInfo GetEmpty()
	{
		FInteractionInfo Info;
		Info.bEnabled = false;
		return Info;
	}
};

UINTERFACE()
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class TOROSYSTEMS_API IInteractionInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, Category = Interaction, DisplayName = "Begin Interact")
		void OnBeginInteract(AToroPlayerCharacter* Player, const FHitResult& HitResult);
	virtual void OnBeginInteract_Implementation(AToroPlayerCharacter* Player, const FHitResult& HitResult) {}

	UFUNCTION(BlueprintNativeEvent, Category = Interaction, DisplayName = "End Interact")
		void OnEndInteract(AToroPlayerCharacter* Player);
	virtual void OnEndInteract_Implementation(AToroPlayerCharacter* Player) {}

	UFUNCTION(BlueprintNativeEvent, Category = Interaction, DisplayName = "Begin Pawn Interact")
		void OnBeginPawnInteract(APawn* Pawn, const FHitResult& HitResult);
	virtual void OnBeginPawnInteract_Implementation(APawn* Pawn, const FHitResult& HitResult) {}

	UFUNCTION(BlueprintNativeEvent, Category = Interaction)
		FInteractionInfo GetInteractInfo(const FHitResult& HitResult);
	virtual FInteractionInfo GetInteractInfo_Implementation(const FHitResult& HitResult) { return {}; }

	static bool ImplementedBy(const UObject* Target) 
	{ 
		return IsValid(Target) ? Target->Implements<UInteractionInterface>() : false; 
	}

	static void BeginInteract(UObject* Target, AToroPlayerCharacter* Player, const FHitResult& HitResult)
	{
		if (ImplementedBy(Target))
		{
			IInteractionInterface::Execute_OnBeginInteract(Target, Player, HitResult);
		}
	}

	static void EndInteract(UObject* Target, AToroPlayerCharacter* Player)
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

	static bool GetInteractInfo(UObject* Target, const FHitResult& HitResult, FInteractionInfo& InteractionInfo)
	{
		if (ImplementedBy(Target))
		{
			InteractionInfo = IInteractionInterface::Execute_GetInteractInfo(Target, HitResult);
			return InteractionInfo.bEnabled;
		}

		return false;
	}
};