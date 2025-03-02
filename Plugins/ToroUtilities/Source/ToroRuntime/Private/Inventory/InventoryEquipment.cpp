// Copyright (C) RedCraft86. All Rights Reserved.

#include "Inventory/InventoryEquipment.h"
#include "Characters/ToroPlayerBase.h"

AInventoryEquipment::AInventoryEquipment()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);
}

void AInventoryEquipment::OnEquip(UInventoryComponent* Inventory, const FInventorySlotData& Slot)
{
	if (const AToroPlayerBase* Player = AToroPlayerBase::Get(Inventory))
	{
		AttachToComponent(Player->EquipmentRoot, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}

	CachedJson.JsonObjectFromString(Slot.Metadata.FindRef(Tag_InvJsonData));
	EquipItem(Inventory, CachedJson);
}

void AInventoryEquipment::OnUnquip(UInventoryComponent* Inventory, FInventorySlotData& Slot)
{
	UnequipItem(Inventory, CachedJson);
	SaveData(Slot);

	SetActorLocation(FVector::ZeroVector);
	GetWorldTimerManager().SetTimerForNextTick([WeakThis = TWeakObjectPtr(this)]()
	{
		if (WeakThis.IsValid()) WeakThis->K2_DestroyActor();
	});
}

void AInventoryEquipment::SaveData(FInventorySlotData& Slot) const
{
	FString JsonStr;
	CachedJson.JsonObjectToString(JsonStr);
	Slot.Metadata.Add(Tag_InvJsonData, JsonStr);
}
