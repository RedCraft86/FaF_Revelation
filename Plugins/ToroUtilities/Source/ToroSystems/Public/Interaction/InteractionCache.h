// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InteractionInterface.h"
#include "InteractionCache.generated.h"

USTRUCT()
struct TOROSYSTEMS_API FInteractionCache
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Cache)
		bool bTriggered;

	UPROPERTY(EditAnywhere, Category = Cache)
		TObjectPtr<AActor> Target;

	UPROPERTY(EditAnywhere, Category = Cache)
		FInteractionInfo Info;

	FInteractionCache(): bTriggered(false), Target(nullptr), Info(FInteractionInfo::GetEmpty()) {}

	void Reset()
	{
		Target = nullptr;
		bTriggered = false;
		Info = FInteractionInfo::GetEmpty();
	}

	void StartInteract(AToroPlayerCharacter* Player, const FHitResult& HitResult)
	{
		if (!bTriggered)
		{
			bTriggered = true;
			IInteractionInterface::BeginInteract(Target, Player, HitResult);
		}
	}

	void StopInteract(AToroPlayerCharacter* Player)
	{
		if (bTriggered)
		{
			bTriggered = false;
			IInteractionInterface::EndInteract(Target, Player);
		}
	}
};