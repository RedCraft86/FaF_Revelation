// Copyright (C) RedCraft86. All Rights Reserved.

#include "GamePlayer.h"

DEFINE_GAMEPLAY_TAG_CHILD(PlayerLock, Dialogue)
DEFINE_GAMEPLAY_TAG_CHILD(PlayerLock, Inventory)

AGamePlayer::AGamePlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	EquipmentRoot = CreateDefaultSubobject<USceneComponent>("EquipmentRoot");
#if WITH_EDITORONLY_DATA
	EquipmentRoot->bVisualizeComponent = true;
#endif

	Narrative = CreateDefaultSubobject<UGameNarrative>("Narrative");
	Inventory = CreateDefaultSubobject<UGameInventory>("Inventory");
}
