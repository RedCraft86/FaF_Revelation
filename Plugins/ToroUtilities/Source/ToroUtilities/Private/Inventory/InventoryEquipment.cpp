// Copyright (C) RedCraft86. All Rights Reserved.

#include "Inventory/InventoryEquipment.h"
#include "Framework/ToroPlayerCharacter.h"
#include "Inventory/InventoryComponent.h"

AInventoryEquipment::AInventoryEquipment()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);
}

void AInventoryEquipment::OnEquip(UInventoryComponent* Inventory, const FInvSlotData& Slot)
{
	if (const AToroPlayerCharacter* Player = AToroPlayerCharacter::Get(Inventory))
	{
		// TODO
		// AttachToComponent(Player->EquipmentRoot, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}

	CachedJson.JsonObjectFromString(Slot.Metadata.FindRef(Tag_InvMeta_JsonData));
	EquipItem(Inventory, CachedJson);
}

void AInventoryEquipment::OnUnquip(UInventoryComponent* Inventory, FInvSlotData& Slot)
{
	UnequipItem(Inventory, CachedJson);
	SaveData(Slot);

	SetActorLocation(FVector::ZeroVector);
	GetWorldTimerManager().SetTimerForNextTick([WeakThis = TWeakObjectPtr(this)]()
	{
		if (WeakThis.IsValid()) WeakThis->K2_DestroyActor();
	});
}

void AInventoryEquipment::SaveData(FInvSlotData& Slot) const
{
	FString JsonStr;
	CachedJson.JsonObjectToString(JsonStr);
	Slot.Metadata.Add(Tag_InvMeta_JsonData, JsonStr);
}
