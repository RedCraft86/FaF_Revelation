// Copyright (C) RedCraft86. All Rights Reserved.

#include "InventoryWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "InventoryComponent.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"

UInventoryWidget::UInventoryWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), bArchiveTab(false)
{
	// TODO ZOrder
	bAutoActivate = false;
}

void UInventoryWidget::SelectItem(const TSoftObjectPtr<UInvItemData>& InItem)
{
	if (bArchiveTab)
	{
		SelectedArchive = InItem;
		for (const TPair<TSoftObjectPtr<UInvItemData>, TObjectPtr<UArchiveSlotWidget>>& Pair : ArchiveWidgets)
		{
			if (Pair.Value) Pair.Value->MarkSelected(Pair.Key == InItem);
		}
	}
	else
	{
		SelectedItem = InItem;
		for (const TPair<TSoftObjectPtr<UInvItemData>, TObjectPtr<UItemSlotWidget>>& Pair : ItemWidgets)
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
	if (UItemSlotWidget* Widget = ItemWidgets.FindRef(SelectedItem))
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
		UInvItemData* ArchiveData = SelectedArchive.LoadSynchronous();
		ThumbnailImage->SetBrushFromTexture(ArchiveData->Icon.LoadSynchronous(), true);
		LabelText->SetText(ArchiveData->DisplayName);
		DescText->SetText(ArchiveData->Description);

		if (!ArchiveData->SecretText.IsEmptyOrWhitespace())
		{
			bool bDiscovered, bSecretFound;
			Inventory->GetArchiveState(ArchiveData, bDiscovered, bSecretFound);
			SecretText->SetVisibility(bSecretFound ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
			SecretText->SetText(FText::Format(INVTEXT("{0}: {1}"),
				ArchiveData->SecretLabel, ArchiveData->SecretText));
		}
	}
	else if (!bArchiveTab && !SelectedItem.IsNull())
	{
		const UInvItemData* ItemData = SelectedItem.LoadSynchronous();
		ThumbnailImage->SetBrushFromTexture(ItemData->Icon.LoadSynchronous(), true);
		LabelText->SetText(ItemData->DisplayName);
		DescText->SetText(ItemData->Description);

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

	const TArray<TSoftObjectPtr<UInvItemData>> Items = Inventory->GetSortedItems();
	for (const TSoftObjectPtr<UInvItemData>& Item : Items)
	{
		if (UItemSlotWidget* Widget = CreateWidget<UItemSlotWidget>(this, ItemWidgetClass))
		{
			Widget->InitWidget(this, Item, Inventory->GetItemAmount(Item), Item == Inventory->GetEquipment());
		}
	}
	SelectedItem = Items.IsEmpty() ? nullptr : Items[0];

	const TArray<TSoftObjectPtr<UInvItemData>> Archives = Inventory->GetSortedArchives();
	for (const TSoftObjectPtr<UInvItemData>& Archive : Archives)
	{
		if (UArchiveSlotWidget* Widget = CreateWidget<UArchiveSlotWidget>(this, ArchiveWidgetClass))
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
	Inventory = UInventoryComponent::Get(this);
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
