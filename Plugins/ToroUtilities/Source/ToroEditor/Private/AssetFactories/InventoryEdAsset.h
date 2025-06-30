// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroEditor.h"
#include "Inventory/InventoryAsset.h"
#include "AssetFactories/ToroAssetFactory.h"
#include "AssetFactories/ToroAssetDefinition.h"
#include "InventoryEdAsset.generated.h"

UCLASS()
class TOROEDITOR_API UInventoryAssetFactory final : public UToroAssetFactory
{
	GENERATED_BODY()

public:

	UInventoryAssetFactory()
	{
		AssetName = TEXT("InventoryAsset");
		SupportedClass = UInventoryAsset::StaticClass();
	}
};

UCLASS()
class TOROEDITOR_API UInventoryAssetDefinition final : public UToroAssetDefinition
{
	GENERATED_BODY()

public:

	UInventoryAssetDefinition()
	{
		Thumbnail.DrawAs = ESlateBrushDrawType::Type::RoundedBox;
		Thumbnail.OutlineSettings.RoundingType = ESlateBrushRoundingType::Type::FixedRadius;
		Thumbnail.OutlineSettings.CornerRadii = FVector4(15.0f, 15.0f, 15.0f, 15.0f);
		Thumbnail.OutlineSettings.Width = 5.0f;
	}

	virtual FLinearColor GetAssetColor() const override { return FColor(100, 25, 160); }
	virtual TSoftClassPtr<UObject> GetAssetClass() const override { return UInventoryAsset::StaticClass(); }
	virtual FText GetAssetDisplayName() const override
	{
		return NSLOCTEXT("ToroEditor", "InventoryAssetName", "Inventory Asset");
	}

	virtual const FSlateBrush* GetThumbnailBrush(const FAssetData& InAssetData, const FName InClassName) const override
	{
		const UInventoryAsset* Asset = Cast<UInventoryAsset>(InAssetData.GetAsset());
		if (UTexture2D* Icon = Asset ? Asset->Icon.LoadSynchronous() : nullptr)
		{
			UInventoryAssetDefinition* NonConstThis = const_cast<UInventoryAssetDefinition*>(this);
			NonConstThis->Thumbnail.ImageSize = FIntPoint(Icon->GetSizeX(), Icon->GetSizeY());
			NonConstThis->Thumbnail.SetResourceObject(Icon);
			return &Thumbnail;
		}
		return nullptr;
	}

	virtual const FSlateBrush* GetIconBrush(const FAssetData& InAssetData, const FName InClassName) const override
	{
		return FToroEditorStyle::Get().GetBrush("ToroEditor.InventoryGridSmall");
	}

private:

	UPROPERTY() FSlateBrush Thumbnail;
};
