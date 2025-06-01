// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Widgets/Layout/SScaleBox.h"
#include "Inventory/InventoryItemData.h"
#include "AssetFactories/ToroAssetFactory.h"
#include "AssetFactories/ToroAssetDefinition.h"
#include "InventoryItemDataAsset.generated.h"

UCLASS()
class FAFREVEDITOR_API UInventoryItemFactory final : public UToroAssetFactory
{
	GENERATED_BODY()

public:

	UInventoryItemFactory()
	{
		AssetName = TEXT("InventoryItem");
		SupportedClass = UInventoryItemData::StaticClass();
	}
};

UCLASS()
class FAFREVEDITOR_API UInventoryItemDefinition final : public UToroAssetDefinition
{
	GENERATED_BODY()

public:

	virtual FLinearColor GetAssetColor() const override { return FColor(0, 255, 200); }
	virtual TSoftClassPtr<UObject> GetAssetClass() const override { return UInventoryItemData::StaticClass(); }
	virtual FText GetAssetDisplayName() const override { return INVTEXT("Inventory Item"); }

	PRAGMA_DISABLE_DEPRECATION_WARNINGS
	virtual TSharedPtr<SWidget> GetThumbnailOverlay(const FAssetData& InAssetData) const override
	{
		FSlateBrush* SmallGrid = new FSlateBrush();
		FSlateBrush* MainBrush = new FSlateBrush();
		MainBrush->DrawAs = ESlateBrushDrawType::Image;

		const UInventoryItemData* Asset = Cast<UInventoryItemData>(InAssetData.GetAsset());
		if (UTexture2D* Icon = IsValid(Asset) ? Asset->Icon.LoadSynchronous() : nullptr)
		{
			SmallGrid->DrawAs = ESlateBrushDrawType::Image;
			SmallGrid->SetResourceObject(LoadObject<UTexture2D>(nullptr,
				TEXT("/ToroUtilities/Icons/InventoryGridSmall.InventoryGridSmall")));
			SmallGrid->SetImageSize(FIntPoint(32));

			MainBrush->SetResourceObject(Icon);
			MainBrush->SetImageSize(FIntPoint(Icon->GetSizeX(), Icon->GetSizeY()));
		}
		else
		{
			SmallGrid->DrawAs = ESlateBrushDrawType::NoDrawType;
			MainBrush->SetResourceObject(LoadObject<UTexture2D>(nullptr,
				TEXT("/ToroUtilities/Icons/InventoryGrid.InventoryGrid")));
			MainBrush->SetImageSize(FIntPoint(32));
		}

		return SNew(SOverlay).Visibility(EVisibility::HitTestInvisible)
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
						SNew(SImage).Image(MainBrush)
					]
				]
			]
			+SOverlay::Slot()
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Bottom)
			.Padding(2.0f, 4.0f)
			[
				SNew(SImage).Image(SmallGrid)
			];
	}
	PRAGMA_ENABLE_DEPRECATION_WARNINGS
};
