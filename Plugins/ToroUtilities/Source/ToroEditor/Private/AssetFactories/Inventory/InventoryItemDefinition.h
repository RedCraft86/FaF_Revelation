// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroEditor.h"
#include "Widgets/Layout/SScaleBox.h"
#include "Inventory/InventoryItemData.h"
#include "AssetFactories/ToroAssetDefinition.h"
#include "InventoryItemDefinition.generated.h"

UCLASS()
class TOROEDITOR_API UInventoryItemDefinition : public UToroAssetDefinition
{
	GENERATED_BODY()

public:
	
	UInventoryItemDefinition() {}
	virtual FLinearColor GetAssetColor() const override { return FColor(100, 25, 160); }
	virtual TSoftClassPtr<UObject> GetAssetClass() const override { return UInventoryItemData::StaticClass(); }
	virtual FText GetAssetDisplayName() const override
	{
		return NSLOCTEXT("ToroEditor", "InventoryItemName", "Inventory Item");
	}

	PRAGMA_DISABLE_DEPRECATION_WARNINGS
	virtual TSharedPtr<SWidget> GetThumbnailOverlay(const FAssetData& InAssetData) const override
	{
		const UInventoryItemData* Asset = Cast<UInventoryItemData>(InAssetData.GetAsset());
		if (UTexture2D* Icon = IsValid(Asset) ? Asset->Thumbnail.LoadSynchronous() : nullptr)
		{
			FSlateBrush* Brush = new FSlateBrush();
			Brush->SetResourceObject(Icon);
			Brush->DrawAs = ESlateBrushDrawType::Image;
			Brush->ImageSize = FIntPoint(Icon->GetSizeX(), Icon->GetSizeY());

			return SNew(SOverlay)
			.Visibility(EVisibility::HitTestInvisible)
			+SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.Padding(0.0f, 0.0f, 0.0f, 2.0f)
			[
				SNew(SBorder)
				.BorderImage(FAppStyle::GetBrush("ContentBrowser.AssetTileItem.ThumbnailAreaBackground"))
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
			+SOverlay::Slot()
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Bottom)
			.Padding(2.0f, 4.0f)
			[
				SNew(SImage).Image(FToroEditorStyle::Get().GetBrush("ToroEditor.InventoryGrid"))
			];
		}

		return Super::GetThumbnailOverlay(InAssetData);
	}
	PRAGMA_ENABLE_DEPRECATION_WARNINGS
};