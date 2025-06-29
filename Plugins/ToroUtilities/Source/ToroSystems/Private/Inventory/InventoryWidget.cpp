// Copyright (C) RedCraft86. All Rights Reserved.

#include "Inventory/InventoryWidget.h"
#include "Inventory/InventoryComponent.h"
#include "UserWidgets/ExprTextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"

UInventoryWidget::UInventoryWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), bArchiveTab(false)
{
	ZOrder = 96;
	bAutoActivate = false;
}

void UInventoryWidget::SelectItem(const TSoftObjectPtr<UInventoryAsset>& InItem)
{
	if (bArchiveTab)
	{
		SelectedArchive = InItem;
		for (const TPair<TSoftObjectPtr<UInventoryAsset>, TObjectPtr<UInvArchiveSlotWidget>>& Pair : ArchiveWidgets)
		{
			if (Pair.Value) Pair.Value->MarkSelected(Pair.Key == InItem);
		}
	}
	else
	{
		SelectedItem = InItem;
		for (const TPair<TSoftObjectPtr<UInventoryAsset>, TObjectPtr<UInvItemSlotWidget>>& Pair : ItemWidgets)
		{
			if (Pair.Value) Pair.Value->MarkSelected(Pair.Key == InItem);
		}
	}

	UpdateInfo();
}

void UInventoryWidget::OnItemTab()
{
	bArchiveTab = false;
	TabSwitch->SetActiveWidgetIndex(0);
	UpdateInfo();
}

void UInventoryWidget::OnArchiveTab()
{
	bArchiveTab = true;
	TabSwitch->SetActiveWidgetIndex(1);
	UpdateInfo();
}

void UInventoryWidget::OnEquipClicked()
{
	const bool bTargetEquip = Inventory->GetEquipment() != SelectedItem;
	bTargetEquip ? Inventory->EquipItem(SelectedItem) : Inventory->UnEquipItem();
	if (const UInvItemSlotWidget* Widget = ItemWidgets.FindRef(SelectedItem))
	{
		Widget->MarkEquipped(bTargetEquip);
	}

	UpdateInfo();
}

void UInventoryWidget::UpdateInfo()
{
	ThumbnailImage->SetVisibility(ESlateVisibility::HitTestInvisible);
	SecretText->SetVisibility(ESlateVisibility::Collapsed);
	EquipBtn->SetVisibility(ESlateVisibility::Collapsed);
	if (bArchiveTab && !SelectedArchive.IsNull())
	{
		UInventoryAsset* ArchiveData = SelectedArchive.LoadSynchronous();
		ThumbnailImage->SetBrushFromTexture(ArchiveData->Icon.LoadSynchronous(), true);
		LabelText->SetText(ArchiveData->DisplayName);
		DescText->SetText(ArchiveData->ContentText);
		ArDescText->SetText(ArchiveData->Description);

		if (!ArchiveData->SecretText.IsEmptyOrWhitespace())
		{
			FInvArchiveSlot ArSlot;
			Inventory->GetArchiveSlot(ArchiveData, ArSlot);
			SecretText->SetVisibility(ArSlot.bKnowSecret
				? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
			SecretText->SetText(ArchiveData->SecretText);
		}
	}
	else if (!bArchiveTab && !SelectedItem.IsNull())
	{
		const UInventoryAsset* ItemData = SelectedItem.LoadSynchronous();
		ThumbnailImage->SetBrushFromTexture(ItemData->Icon.LoadSynchronous(), true);
		LabelText->SetText(ItemData->DisplayName);
		DescText->SetText(ItemData->Description);
		ArDescText->SetText(FText::GetEmpty());

		if (ItemData->ItemType == EInvItemType::Equipment)
		{
			EquipBtn->SetVisibility(ESlateVisibility::Visible);
			EquipLabelText->SetText(Inventory->GetEquipment() == SelectedItem
				? INVTEXT("Unequip Item") : INVTEXT("Equip Item"));
		}
	}
	else
	{
		LabelText->SetText(INVTEXT("None Selected"));
		DescText->SetText(INVTEXT("Select something to view the description"));
		ThumbnailImage->SetVisibility(ESlateVisibility::Collapsed);
	}

	PlayAnimation(InfoAnim);
}

void UInventoryWidget::UpdateSlots()
{
	ItemWidgets.Empty();
	ArchiveWidgets.Empty();

	ItemBox->ClearChildren();
	ArchiveBox->ClearChildren();

	const TArray<TSoftObjectPtr<UInventoryAsset>> Items = Inventory->GetSortedItems();
	for (const TSoftObjectPtr<UInventoryAsset>& Item : Items)
	{
		if (UInvItemSlotWidget* Widget = CreateWidget<UInvItemSlotWidget>(this, ItemWidgetClass))
		{
			Widget->InitWidget(this, Item, Inventory->GetItemAmount(Item),
				Item == Inventory->GetEquipment());
		}
	}
	SelectedItem = Items.IsEmpty() ? nullptr : Items[0];

	const TArray<TSoftObjectPtr<UInventoryAsset>> Archives = Inventory->GetSortedArchives();
	for (const TSoftObjectPtr<UInventoryAsset>& Archive : Archives)
	{
		if (UInvArchiveSlotWidget* Widget = CreateWidget<UInvArchiveSlotWidget>(this, ArchiveWidgetClass))
		{
			Widget->InitWidget(this, Archive);
		}
	}
	SelectedArchive = Archives.IsEmpty() ? nullptr : Archives[0];

	PlayAnimation(SlotAnim);
	UpdateInfo();
}

void UInventoryWidget::InitWidget()
{
	Super::InitWidget();
	ItemBtn->OnClicked.AddDynamic(this, &UInventoryWidget::OnItemTab);
	ArchiveBtn->OnClicked.AddDynamic(this, &UInventoryWidget::OnArchiveTab);
	EquipBtn->OnClicked.AddDynamic(this, &UInventoryWidget::OnEquipClicked);
}

void UInventoryWidget::InternalProcessActivation()
{
	Super::InternalProcessActivation();
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		if (!Inventory) Inventory = UInventoryComponent::Get(this);
		UpdateSlots();
	});
}
