// Copyright (C) RedCraft86. All Rights Reserved.

#include "Inventory/EquipmentActor.h"

AEquipmentActor::AEquipmentActor(): bUsing(false)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);
}

void AEquipmentActor::SaveData(FString& InJson)
{
	OnDataSave(JsonData);
	JsonData.JsonObjectToString(InJson);
}

void AEquipmentActor::LoadData(const FString& InJson)
{
	JsonData.JsonObjectFromString(InJson);
	OnDataLoad(JsonData);
}

void AEquipmentActor::SetUseState(const bool bInUse)
{
	if (bUsing != bInUse)
	{
		bUsing = bInUse;
		bUsing ? OnBeginUse() : OnEndUse();
	}
}
