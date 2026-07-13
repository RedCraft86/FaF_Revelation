// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "EquipmentActor.h"

AEquipmentActor::AEquipmentActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;
}

void AEquipmentActor::BeginPlay()
{
	Super::BeginPlay();
	SetActorEnableCollision(false);
}
