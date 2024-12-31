﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "PrimitiveData.h"
#include "InventoryItemAttributes.generated.h"

USTRUCT(BlueprintInternalUseOnly)
struct TOROGAMEPLAY_API FInventoryItemAttribute
{
	GENERATED_BODY()

	FInventoryItemAttribute() {}
	virtual ~FInventoryItemAttribute() = default;
};

USTRUCT(DisplayName = "Alt Names")
struct TOROGAMEPLAY_API FInvItemAltName : public FInventoryItemAttribute
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
struct TOROGAMEPLAY_API FInvItemAltDescription : public FInventoryItemAttribute
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
struct TOROGAMEPLAY_API FInvItemAltPreview : public FInventoryItemAttribute
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Attribute)
		TMap<FString, FTransformMeshData> AltMeshes;
	
	FInvItemAltPreview() {}
};

USTRUCT(DisplayName = "Viewable")
struct TOROGAMEPLAY_API FInvItemViewable : public FInventoryItemAttribute
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Attribute)
		FText Label;
	
	UPROPERTY(EditAnywhere, Category = Attribute)
		TObjectPtr<UTexture2D> Image;
	
	UPROPERTY(EditAnywhere, Category = Attribute)
		FText Text;
	
	FInvItemViewable() : Label(INVTEXT("View")) {}
};

USTRUCT(DisplayName = "Consumable")
struct TOROGAMEPLAY_API FInvItemConsumable : public FInventoryItemAttribute
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Attribute)
		FText Label;
	
	UPROPERTY(EditAnywhere, Category = Attribute)
		TSubclassOf<UObject> Class;
	
	FInvItemConsumable() : Label(INVTEXT("Use")) {}
};

USTRUCT(DisplayName = "Equipment")
struct TOROGAMEPLAY_API FInvItemEquipment : public FInventoryItemAttribute
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Attribute)
		FText EquipLabel;

	UPROPERTY(EditAnywhere, Category = Attribute)
		FText UnequipLabel;
	
	UPROPERTY(EditAnywhere, Category = Attribute)
		TSubclassOf<AActor> Class;
	
	FInvItemEquipment() : EquipLabel(INVTEXT("Equip")), UnequipLabel(INVTEXT("Unequip")) {}
};
