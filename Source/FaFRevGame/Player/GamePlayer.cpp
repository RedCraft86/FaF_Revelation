// Copyright (C) RedCraft86. All Rights Reserved.

#include "GamePlayer.h"

AGamePlayer::AGamePlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	EquipmentRoot = CreateDefaultSubobject<UGameNarrative>("EquipmentRoot");
#if WITH_EDITORONLY_DATA
	EquipmentRoot->bVisualizeComponent = true;
#endif

	Narrative = CreateDefaultSubobject<UGameNarrative>("Narrative");
	Inventory = CreateDefaultSubobject<UGameInventory>("Inventory");
}
