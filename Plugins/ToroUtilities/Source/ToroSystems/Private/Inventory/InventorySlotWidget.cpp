// Copyright (C) RedCraft86. All Rights Reserved.

#include "Inventory/InventorySlotWidget.h"
#include "Inventory/InventoryWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

FReply UInvSlotWidget::NativeOnMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		ParentWidget->SelectItem(InvAsset);
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

void UInvItemSlotWidget::MarkEquipped(const bool bEquipped) const
{
	EquipImage->SetVisibility(bEquipped ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void UInvItemSlotWidget::InitWidget(UInventoryWidget* Owner, const TSoftObjectPtr<UInventoryAsset>& InItem, const uint8 Amount, const bool bEquipped)
{
	InvAsset = InItem;
	ParentWidget = Owner;
	AmountText->SetText(FText::FromString(FString::FromInt(Amount)));
	ThumbnailImage->SetBrushFromTexture(InvAsset->Icon.LoadSynchronous(), true);
	MarkEquipped(bEquipped);
}

void UInvArchiveSlotWidget::InitWidget(UInventoryWidget* Owner, const TSoftObjectPtr<UInventoryAsset>& InItem)
{
	InvAsset = InItem;
	ParentWidget = Owner;
	ItemText->SetText(InvAsset->DisplayName);
}
