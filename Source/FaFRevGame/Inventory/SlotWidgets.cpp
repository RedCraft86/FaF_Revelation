// Copyright (C) RedCraft86. All Rights Reserved.

#include "SlotWidgets.h"
#include "InventoryWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

FReply USlotWidgetBase::NativeOnMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		ParentWidget->SelectItem(ItemData);
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

void UItemSlotWidget::MarkEquipped(const bool bEquipped)
{
	EquipImage->SetVisibility(bEquipped ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void UItemSlotWidget::InitWidget(UInventoryWidget* Owner, const TSoftObjectPtr<UInvItemData>& InItem, const uint8 Amount, const bool bEquipped)
{
	ItemData = InItem;
	ParentWidget = Owner;
	AmountText->SetText(FText::FromString(FString::FromInt(Amount)));
	ThumbnailImage->SetBrushFromTexture(ItemData->Icon.LoadSynchronous(), true);
	MarkEquipped(bEquipped);
}

void UArchiveSlotWidget::InitWidget(UInventoryWidget* Owner, const TSoftObjectPtr<UInvItemData>& InItem)
{
	ItemData = InItem;
	ParentWidget = Owner;
	ItemText->SetText(ItemData->DisplayName);
}
