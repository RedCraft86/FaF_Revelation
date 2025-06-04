// Copyright (C) RedCraft86. All Rights Reserved.

#include "InventoryWidget.h"
#include "GameInventory.h"
#include "IDetailTreeNode.h"

void UInvSlotWidget::OnClicked()
{
	ParentUI->SelectSlot(SlotItem);
}

void UInvSlotWidget::InitData(const TSoftObjectPtr<UInventoryItemData>& Item, const uint8 Amount, const bool bEquipped)
{
	SlotItem = Item;
	if (const UInventoryItemData* ItemObj = Item.LoadSynchronous())
	{
		SlotButton->OnClicked.AddDynamic(this, &ThisClass::OnClicked);
		ThumbnailImage->SetBrushFromTexture(Item->Icon.LoadSynchronous(), false);
		if (ItemObj->ItemType == EInventoryItemType::Archive)
		{
			ItemText->SetText(Item->DisplayName);
			ItemText->SetVisibility(ESlateVisibility::HitTestInvisible);
			AmountText->SetVisibility(ESlateVisibility::Collapsed);
			EquipImage->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			SlotButton->SetToolTipText(Item->DisplayName);
			ItemText->SetVisibility(ESlateVisibility::Collapsed);
			AmountText->SetText(FText::Format(INVTEXT("{0}/{1}"), Amount, Item->StackSize));
			EquipImage->SetVisibility(bEquipped ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		}
	}
}

void UInvSlotWidget::SetEquipped(bool bEquipped)
{
	EquipImage->SetVisibility(bEquipped ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void UInventoryWidget::SelectSlot(const TSoftObjectPtr<UInventoryItemData>& InKey)
{
	if (bArchiveTab)
	{
		SelectedArchive = InKey;
		for (const TPair<TSoftObjectPtr<UInventoryItemData>, TObjectPtr<UInvSlotWidget>>& Archive : ArchiveWidgets)
		{
			if (Archive.Value) Archive.Value->MarkSelected(Archive.Key == SelectedArchive);
		}
	}
	else
	{
		SelectedItem = InKey;
		for (const TPair<TSoftObjectPtr<UInventoryItemData>, TObjectPtr<UInvSlotWidget>>& Item : ItemWidgets)
		{
			if (Item.Value) Item.Value->MarkSelected(Item.Key == SelectedItem);
		}
	}

	RefreshInfo();
}

void UInventoryWidget::GoToArchive(const TSoftObjectPtr<UInventoryItemData>& InKey)
{
	OnArchiveTab();
	SelectSlot(InKey);
}

void UInventoryWidget::OnItemTab()
{
	bArchiveTab = false;
	TabSwitch->SetActiveWidgetIndex(0);
	RefreshInfo();
}

void UInventoryWidget::OnArchiveTab()
{
	bArchiveTab = true;
	TabSwitch->SetActiveWidgetIndex(1);
	RefreshInfo();
}

void UInventoryWidget::OnEquipClicked()
{
	bool bTargetEquip = Inventory->GetEquipment() != SelectedItem;

	bTargetEquip ? Inventory->EquipItem(SelectedItem) : Inventory->UnEquipItem();
	if (UInvSlotWidget* Widget = ItemWidgets.FindRef(SelectedItem))
	{
		Widget->SetEquipped(bTargetEquip);
	}

	RefreshInfo();
}

void UInventoryWidget::RefreshSlots()
{
	ItemWidgets.Empty();
	ArchiveWidgets.Empty();

	ItemBox->ClearChildren();
	ArchiveBox->ClearChildren();

	const TArray<TSoftObjectPtr<UInventoryItemData>> Items = Inventory->GetSortedItems();
	for (const TSoftObjectPtr<UInventoryItemData>& Item : Items)
	{
		if (UInvSlotWidget* Widget = CreateWidget<UInvSlotWidget>(this, ItemWidgetClass))
		{
			Widget->InitData(Item, Inventory->GetItemAmount(Item), Item == Inventory->GetEquipment());
		}
	}
	if (!Items.IsEmpty()) SelectedItem = Items[0];
	
	const TArray<TSoftObjectPtr<UInventoryItemData>> Archives = Inventory->GetSortedArchives();
	for (const TSoftObjectPtr<UInventoryItemData>& Archive : Archives)
	{
		if (UInvSlotWidget* Widget = CreateWidget<UInvSlotWidget>(this, ArchiveWidgetClass))
		{
			Widget->InitData(Archive, 1, false);
		}
	}
	if (!Archives.IsEmpty()) SelectedArchive = Archives[0];

	RefreshInfo();
}

void UInventoryWidget::RefreshInfo() const
{
	NameText->SetVisibility(ESlateVisibility::Collapsed);
	DescText->SetVisibility(ESlateVisibility::Collapsed);
	ViewImage->SetVisibility(ESlateVisibility::Collapsed);
	EquipButton->SetVisibility(ESlateVisibility::Collapsed);
	if (bArchiveTab)
	{
		if (const UInventoryItemData* Archive = SelectedArchive.LoadSynchronous())
		{
			NameText->SetText(Archive->DisplayName);
			NameText->SetVisibility(ESlateVisibility::HitTestInvisible);
			
			DescText->SetText(Archive->Description);
			DescText->SetVisibility(ESlateVisibility::HitTestInvisible);

			ViewImage->SetBrushFromTexture(Archive->Icon.LoadSynchronous(), true);
			ViewImage->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
	else if (const UInventoryItemData* Item = SelectedItem.LoadSynchronous())
	{
		NameText->SetText(Item->DisplayName);
		NameText->SetVisibility(ESlateVisibility::HitTestInvisible);
			
		DescText->SetText(Item->Description);
		DescText->SetVisibility(ESlateVisibility::HitTestInvisible);

		ViewImage->SetBrushFromTexture(Item->Icon.LoadSynchronous(), true);
		ViewImage->SetVisibility(ESlateVisibility::HitTestInvisible);

		if (Item->ItemType == EInventoryItemType::Equipment)
		{
			EquipButton->SetVisibility(ESlateVisibility::Visible);
			EquipLabelText->SetText(Inventory->GetEquipment() == SelectedItem
				? INVTEXT("Unequip") : INVTEXT("Equip"));
		}
	}
}

void UInventoryWidget::InitWidget()
{
	Super::InitWidget();
	Inventory = UGameInventory::Get(this);

	ItemBtn->OnClicked.AddDynamic(this, &ThisClass::OnItemTab);
	ArchiveBtn->OnClicked.AddDynamic(this, &ThisClass::OnArchiveTab);
	EquipButton->OnClicked.AddDynamic(this, &ThisClass::OnEquipClicked);
}

void UInventoryWidget::InternalProcessActivation()
{
	Super::InternalProcessActivation();
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		if (!Inventory) Inventory = UGameInventory::Get(this);
		RefreshSlots();
	});
}
