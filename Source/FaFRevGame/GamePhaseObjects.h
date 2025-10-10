// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Characters/Player/PlayerCharacter.h"
#include "GamePhase/GamePhaseData.h"
#include "GamePhaseObjects.generated.h"

UCLASS()
class TORORUNTIME_API UGamePhaseNode final : public UToroGamePhaseNode
{
	GENERATED_BODY()

public:

	UGamePhaseNode();

protected:

	UPROPERTY(EditAnywhere, Category = Player, meta = (Bitmask, BitmaskEnum = "/Script/FaFRevGame.EPlayerControlFlags"))
		int32 PlayerAbilities;

	virtual void ApplyPlayerSettings(AToroPlayerCharacter* PlayerChar) const override
	{
		Super::ApplyPlayerSettings(PlayerChar);
		if (APlayerCharacter* Player = Cast<APlayerCharacter>(PlayerChar))
		{
			// TODO apply abilities
		}
	}
};

UCLASS()
class TORORUNTIME_API UGamePhaseGraph final : public UToroGamePhaseGraph
{
	GENERATED_BODY()

public:

	UGamePhaseGraph()
	{
		CompatibleNode = UGamePhaseNode::StaticClass();
	}
};