// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

class AGamePlayerChar;
class AToroPlayerCharacter;

USTRUCT(BlueprintType)
struct FAFREVGAME_API FInteractionInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		bool bEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (EditCondition = "bEnabled", MultiLine = true))
		FText Label;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (EditCondition = "bEnabled"))
		FVector2D LabelOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (EditCondition = "bEnabled"))
		TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (EditCondition = "bEnabled", AllowPreserveRatio = true, ClampMin = 0.1f, UIMin = 0.1f))
		FVector2D IconSize;
	
	FInteractionInfo()
		: bEnabled(true), Label(INVTEXT("Interact")), LabelOffset(0.0f), Icon(nullptr), IconSize(35.0f)
	{}

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

class FAFREVGAME_API IInteractionInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, Category = Interaction, DisplayName = "Begin Interact")
		void OnBeginInteract(AGamePlayerChar* Player, const FHitResult& HitResult);
	virtual void OnBeginInteract_Implementation(AGamePlayerChar* Player, const FHitResult& HitResult) {}

	UFUNCTION(BlueprintNativeEvent, Category = Interaction, DisplayName = "End Interact")
		void OnEndInteract(AGamePlayerChar* Player);
	virtual void OnEndInteract_Implementation(AGamePlayerChar* Player) {}

	UFUNCTION(BlueprintNativeEvent, Category = Interaction, DisplayName = "Begin Pawn Interact")
		void OnBeginPawnInteract(APawn* Pawn, const FHitResult& HitResult);
	virtual void OnBeginPawnInteract_Implementation(APawn* Pawn, const FHitResult& HitResult) {}

	UFUNCTION(BlueprintNativeEvent, Category = Interaction)
		FInteractionInfo GetInteractionInfo(const FHitResult& HitResult);
	virtual FInteractionInfo GetInteractionInfo_Implementation(const FHitResult& HitResult) { return {}; }

	static bool ImplementedBy(const UObject* Target) 
	{ 
		return IsValid(Target) ? Target->Implements<UInteractionInterface>() : false; 
	}

	static void BeginInteract(UObject* Target, AGamePlayerChar* Player, const FHitResult& HitResult)
	{
		if (ImplementedBy(Target))
		{
			IInteractionInterface::Execute_OnBeginInteract(Target, Player, HitResult);
		}
	}

	static void EndInteract(UObject* Target, AGamePlayerChar* Player)
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

	static bool GetInteractionInfo(UObject* Target, const FHitResult& HitResult, FInteractionInfo& InteractionInfo)
	{
		if (ImplementedBy(Target))
		{
			InteractionInfo = IInteractionInterface::Execute_GetInteractionInfo(Target, HitResult);
			return InteractionInfo.bEnabled;
		}

		return false;
	}
};