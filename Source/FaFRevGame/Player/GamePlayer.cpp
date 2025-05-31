// Copyright (C) RedCraft86. All Rights Reserved.

#include "GamePlayer.h"

AGamePlayer::AGamePlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	Narrative = CreateDefaultSubobject<UGameNarrative>("Narrative");
	Inventory = CreateDefaultSubobject<UGameInventory>("Inventory");
}
