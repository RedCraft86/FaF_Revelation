// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Player/PlayerTypes.h"
#include "Player/PlayerCharacter.h"
#include "GamePhase/GamePhaseData.h"
#include "GamePhaseObjects.generated.h"

UCLASS()
class UGamePhaseNode final : public UToroGamePhaseNode
{
	GENERATED_BODY()

public:

	UGamePhaseNode(): PlayerAbilities(Player::DefaultAbilities) {}

protected:

	UPROPERTY(EditAnywhere, Category = Player, meta = (DisplayPriority = 0, Bitmask, BitmaskEnum = "/Script/FaFRevGame.EPlayerControlFlags"))
		int32 PlayerAbilities;

	virtual void ApplyPlayerSettings(AToroPlayerCharacter* PlayerChar) const override
	{
		Super::ApplyPlayerSettings(PlayerChar);
		if (APlayerCharacter* Player = Cast<APlayerCharacter>(PlayerChar))
		{
			Player->OverrideControlFlags(PlayerAbilities);
		}
	}
};

UCLASS()
class UGamePhaseGraph final : public UToroGamePhaseGraph
{
	GENERATED_BODY()

public:

	UGamePhaseGraph()
	{
		CompatibleNode = UGamePhaseNode::StaticClass();
	}
};