// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InventorySlot.h"
#include "EquipmentActor.h"
#include "InventoryAsset.h"
#include "Components/ActorComponent.h"
#include "DataTypes/InputModeData.h"
#include "InventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct TOROSYSTEMS_API FInventoryData
{
	GENERATED_BODY()

	UPROPERTY(Transient) TSoftObjectPtr<UInventoryAsset> Equipment;
	UPROPERTY(Transient) TMap<TSoftObjectPtr<UInventoryAsset>, FInvItemSlot> Items;
	UPROPERTY(Transient) TMap<TSoftObjectPtr<UInventoryAsset>, FInvArchiveSlot> Archives;

	FInventoryData() {}
	FInventoryData(const TSoftObjectPtr<UInventoryAsset>& InEquipment,
		const TMap<TSoftObjectPtr<UInventoryAsset>, FInvItemSlot>& InItems,
		const TMap<TSoftObjectPtr<UInventoryAsset>, FInvArchiveSlot>& InArchives)
			: Equipment(InEquipment), Items(InItems), Archives(InArchives)
	{}

	friend FArchive& operator<<(FArchive& Ar, FInventoryData& Data)
	{
		Ar << Data.Items;
		Ar << Data.Archives;
		Ar << Data.Equipment;
		return Ar;
	}
	
	void EnsureData(const TMap<TSoftObjectPtr<UInventoryAsset>, uint8>& InItems,
		const TMap<TSoftObjectPtr<UInventoryAsset>, bool>& InArchives)
	{
		for (const TPair<TSoftObjectPtr<UInventoryAsset>, uint8>& Item : InItems)
		{
			if (Item.Key.IsNull()) continue;
			FInvItemSlot& Slot = Items.FindOrAdd(Item.Key);
			Slot.Amount = FMath::Max(Item.Value, Slot.Amount);
		}

		for (const TPair<TSoftObjectPtr<UInventoryAsset>, bool>& Archive : InArchives)
		{
			if (Archive.Key.IsNull()) continue;
			FInvArchiveSlot& Slot = Archives.FindOrAdd(Archive.Key);
			Slot.bKnowSecret = Slot.bKnowSecret || Archive.Value;
		}
	}
};

UCLASS(NotBlueprintable, ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class TOROSYSTEMS_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UInventoryComponent();

protected:

	UPROPERTY() bool bInInventory;
	UPROPERTY() FInventoryData InvData;
	UPROPERTY() FGameInputModeData CachedInput;
	UPROPERTY(Transient) TObjectPtr<AEquipmentActor> Equipment;

	virtual void BeginPlay() override;
};
