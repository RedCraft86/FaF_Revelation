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

FJsonObjectWrapper AEquipmentActor::UnequipItem()
{
	FJsonObjectWrapper Json;
	OnSaveData(Json);
	GetWorldTimerManager().SetTimerForNextTick([this]()
	{
		OnUnequip();
	});
	return Json;
}

void AEquipmentActor::EquipItem(const FJsonObjectWrapper& InMetadata)
{
	OnLoadData(InMetadata);
	GetWorldTimerManager().SetTimerForNextTick([this]()
	{
		OnEquip();
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
