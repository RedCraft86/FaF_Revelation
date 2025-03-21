// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

class AToroPlayerCharacter;

UENUM(BlueprintType)
enum class EInteractableType : uint8
{
	// No interaction
	None,
	
	// Immediately trigger the interaction
	Instant,

	// Trigger the interaction after holding for a set time
	Held
};

USTRUCT(BlueprintType)
struct TOROUTILITIES_API FInteractionInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		EInteractableType Interaction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (EditCondition = "Interaction == EInteractableType::Held", ClampMin = 0.5f, UIMin = 0.5f))
		float HoldingTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (EditCondition = "Interaction != EInteractableType::None", MultiLine = true))
		FText Label;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (EditCondition = "Interaction != EInteractableType::None"))
		FVector2D LabelOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (EditCondition = "Interaction != EInteractableType::None"))
		TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (EditCondition = "Interaction != EInteractableType::None", AllowPreserveRatio = true, ClampMin = 0.1f, UIMin = 0.1f))
		FVector2D IconSize;
	
	FInteractionInfo() : Interaction(EInteractableType::Instant), HoldingTime(1.0f)
		, Label(NSLOCTEXT("ToroUtilities", "Interact", "Interact")), LabelOffset(0.0f), Icon(nullptr), IconSize(35.0f)
	{}

	bool CanInteract() const { return Interaction != EInteractableType::None; }

	static FInteractionInfo GetEmpty()
	{
		FInteractionInfo Info;
		Info.Interaction = EInteractableType::None;
		return Info;
	}
};

UINTERFACE()
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class TOROUTILITIES_API IInteractionInterface
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
		FInteractionInfo GetInteractionInfo(const FHitResult& HitResult);
	virtual FInteractionInfo GetInteractionInfo_Implementation(const FHitResult& HitResult) { return {}; }

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

	static bool GetInteractionInfo(UObject* Target, const FHitResult& HitResult, FInteractionInfo& InteractionInfo)
	{
		if (ImplementedBy(Target))
		{
			InteractionInfo = IInteractionInterface::Execute_GetInteractionInfo(Target, HitResult);
			return InteractionInfo.CanInteract();
		}

		return false;
	}
};
