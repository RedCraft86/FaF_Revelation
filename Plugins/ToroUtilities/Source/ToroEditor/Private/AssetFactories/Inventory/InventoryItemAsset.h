// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroEditor.h"
#include "AssetFactories/ToroAssetTypeActions.h"
#include "Inventory/InventoryItemData.h"
#include "Widgets/Layout/SScaleBox.h"

class FInventoryItemAsset final : public FToroAssetTypeActions
{
public:

	explicit FInventoryItemAsset() {}
	virtual FColor GetTypeColor() const override { return FColor(100, 25, 160); }
	virtual UClass* GetSupportedClass() const override { return UInventoryItemData::StaticClass(); }
	virtual FText GetName() const override { return NSLOCTEXT("ToroEditor", "InventoryItemName", "Inventory Item"); }
	virtual TSharedPtr<SWidget> GetThumbnailOverlay(const FAssetData& AssetData) const override
	{
		const UInventoryItemData* Asset = Cast<UInventoryItemData>(AssetData.GetAsset());
		if (UTexture2D* Icon = IsValid(Asset) ? Asset->Thumbnail : nullptr)
		{
			FSlateBrush* Brush = new FSlateBrush();
			Brush->SetResourceObject(Icon);
			Brush->DrawAs = ESlateBrushDrawType::Image;
			Brush->ImageSize = FIntPoint(Icon->GetSizeX(), Icon->GetSizeY());

			return SNew(SOverlay)
				.Visibility(EVisibility::HitTestInvisible)
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.Padding(0.0f, 0.0f, 0.0f, 2.0f)
				[
					SNew(SBorder)
					.BorderImage(FAppStyle::GetBrush(TEXT("ContentBrowser.AssetTileItem.ThumbnailAreaBackground")))
					.Padding(FMargin(10.0f))
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						SNew(SScaleBox)
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						.Stretch(EStretch::ScaleToFit)
						[
							SNew(SImage).Image(Brush)
						]
					]
				]
				+ SOverlay::Slot()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Bottom)
				.Padding(2.0f, 4.0f)
				[
					SNew(SImage).Image(FToroEditorStyle::Get().GetBrush(TEXT("ToroEditor.InventoryGrid")))
				];
		}

		return FAssetTypeActions_Base::GetThumbnailOverlay(AssetData);
	}
};