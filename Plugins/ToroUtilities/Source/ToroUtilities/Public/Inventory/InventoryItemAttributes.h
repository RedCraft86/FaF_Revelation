// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "PrimitiveData.h"
#include "InventoryItemAttributes.generated.h"

USTRUCT(BlueprintInternalUseOnly)
struct TOROUTILITIES_API FInventoryItemAttribute
{
	GENERATED_BODY()

	FInventoryItemAttribute() {}
	virtual ~FInventoryItemAttribute() = default;
};

USTRUCT(DisplayName = "Alt Names")
struct TOROUTILITIES_API FInvItemAltName : public FInventoryItemAttribute
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Attribute)
		TMap<FString, FText> AltNames;
	
	FInvItemAltName() {}

	void GetName(const FString& InKey, FText& OutText) const
	{
		if (InKey.IsEmpty()) return;
		const FText* Result = AltNames.Find(InKey);
		if (Result && !Result->IsEmptyOrWhitespace()) OutText = *Result;
	}
};

USTRUCT(DisplayName = "Alt Descriptions")
struct TOROUTILITIES_API FInvItemAltDescription : public FInventoryItemAttribute
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Attribute)
		TMap<FString, FText> AltDescriptions;
	
	FInvItemAltDescription() {}

	void GetDescription(const FString& InKey, FText& OutText) const
	{
		if (InKey.IsEmpty()) return;
		const FText* Result = AltDescriptions.Find(InKey);
		if (Result && !Result->IsEmptyOrWhitespace()) OutText = *Result;
	}
};

USTRUCT(DisplayName = "Alt Previews")
struct TOROUTILITIES_API FInvItemAltPreview : public FInventoryItemAttribute
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Attribute)
		TMap<FString, FTransformMeshData> AltMeshes;
	
	FInvItemAltPreview() {}

	void GetMeshData(const FString& InKey, FTransformMeshData& OutData) const
	{
		if (InKey.IsEmpty()) return;
		const FTransformMeshData* Result = AltMeshes.Find(InKey);
		if (Result && Result->IsValidData()) OutData = *Result;
	}
};

USTRUCT(DisplayName = "Viewable")
struct TOROUTILITIES_API FInvItemViewable : public FInventoryItemAttribute
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Attribute)
		FText Label;
	
	UPROPERTY(EditAnywhere, Category = Attribute)
		TSoftObjectPtr<UTexture2D> Image;
	
	UPROPERTY(EditAnywhere, Category = Attribute)
		FText Text;
	
	FInvItemViewable() : Label(NSLOCTEXT("Toro", "ReadView", "Read/View")) {}
	bool IsValidData() const { return !Image.IsNull() || !Text.IsEmptyOrWhitespace(); }
};

USTRUCT(DisplayName = "Consumable")
struct TOROUTILITIES_API FInvItemConsumable : public FInventoryItemAttribute
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Attribute)
		FText Label;

	UPROPERTY(EditAnywhere, Category = Attribute)
		uint8 Amount;
	
	UPROPERTY(EditAnywhere, Category = Attribute)
		TSoftClassPtr<class UInventoryConsumable> Class;
	
	FInvItemConsumable() : Label(NSLOCTEXT("Toro", "Use", "Use")), Amount(1) {}
	bool IsValidData() const { return !Class.IsNull(); }
};

USTRUCT(DisplayName = "Equipment")
struct TOROUTILITIES_API FInvItemEquipment : public FInventoryItemAttribute
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Attribute)
		FText EquipLabel;

	UPROPERTY(EditAnywhere, Category = Attribute)
		FText UnequipLabel;
	
	UPROPERTY(EditAnywhere, Category = Attribute)
		TSoftClassPtr<class AInventoryEquipment> Class;
	
	FInvItemEquipment() : EquipLabel(NSLOCTEXT("Toro", "Equip", "Equip"))
		, UnequipLabel(NSLOCTEXT("Toro", "Unequip", "Unequip"))
	{}
	bool IsValidData() const { return !Class.IsNull(); }
};
