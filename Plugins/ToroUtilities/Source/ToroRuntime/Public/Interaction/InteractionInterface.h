// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

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
struct TORORUNTIME_API FInteractableInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		EInteractableType Interaction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (EditCondition = "Interaction == EInteractableType::Held"))
		float HoldingTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (EditCondition = "Interaction != EInteractableType::None", MultiLine = true))
		FText Label;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (EditCondition = "Interaction != EInteractableType::None"))
		FVector2D LabelOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (EditCondition = "Interaction != EInteractableType::None"))
		TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (EditCondition = "Interaction != EInteractableType::None", AllowPreserveRatio = true, ClampMin = 0.1f, UIMin = 0.1f))
		FVector2D IconSize;
	
	FInteractableInfo() : Interaction(EInteractableType::Instant), HoldingTime(1.0f)
		, Label(INVTEXT("Interact")), LabelOffset(0.0f), Icon(nullptr), IconSize(35.0f)
	{}
};

UINTERFACE()
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class TORORUNTIME_API IInteractionInterface
{
	GENERATED_BODY()

public:

	
};

namespace IInteract
{
	static bool ImplementedBy(const UObject* Target) 
	{ 
		return IsValid(Target) ? Target->Implements<UInteractionInterface>() : false; 
	}
}