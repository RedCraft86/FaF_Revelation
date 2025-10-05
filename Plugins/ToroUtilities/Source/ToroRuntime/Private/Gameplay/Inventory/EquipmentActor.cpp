// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/Inventory/EquipmentActor.h"

AEquipmentActor::AEquipmentActor(): bUsing(false)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);
}

void AEquipmentActor::EquipItem()
{
	GetWorldTimerManager().SetTimerForNextTick([this]()
	{
		OnEquip();
	});
}

void AEquipmentActor::UnequipItem()
{
	SetUseState(false);
	GetWorldTimerManager().SetTimerForNextTick([this]()
	{
		OnUnequip();
	});
}

void AEquipmentActor::SetUseState(const bool bInUse)
{
	if (bUsing != bInUse)
	{
		bUsing = bInUse;
		bUsing ? OnBeginUse() : OnEndUse();
	}
}
