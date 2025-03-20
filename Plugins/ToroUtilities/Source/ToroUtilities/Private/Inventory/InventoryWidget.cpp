﻿
// Copyright (C) RedCraft86. All Rights Reserved.

// ReSharper disable CppMemberFunctionMayBeConst
#include "Inventory/InventoryWidget.h"
#include "Inventory/InventoryPreview.h"
#include "Inventory/InventoryItemData.h"
#include "Inventory/InventoryComponent.h"
#include "Framework/ToroPlayerController.h"
#include "Animation/WidgetAnimation.h"
#include "Components/ComboBoxString.h"
#include "UserWidgets/ExprTextBlock.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"
#include "Components/Button.h"
#include "Components/Image.h"

void UInvSlotWidget::OnClicked()
{
	ParentUI->SelectSlot(SlotID);
}

void UInvSlotWidget::MarkSelected(const bool bSelected) const
{
	OutlineImage->SetColorAndOpacity(bSelected ? SelectedColor : NormalColor);
}

void UInvSlotWidget::InitData(const FGuid& InSlotID, const FInvSlotData& SlotData, const bool bEquipped)
{
	SlotID = InSlotID;
	const UInventoryItemData* Item = SlotData.Item.LoadSynchronous();

	ThumbnailImage->SetBrushFromTexture(Item->Thumbnail.LoadSynchronous(), false);
	EquipImage->SetVisibility(bEquipped ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	OutlineImage->SetColorAndOpacity(NormalColor);

	AmountText->SetText(FText::Format(INVTEXT("{0}/{1}"), SlotData.Amount, Item->GetStackLimit()));
	SlotButton->SetToolTipText(Item->GetDisplayName(SlotData.Metadata));
	SlotButton->OnClicked.AddDynamic(this, &ThisClass::OnClicked);
}

void UInventoryWidget::SelectSlot(const FGuid& InKey)
{
	if (!Inventory) return;
	if (SelectedKey == InKey)
	{
		if (const TObjectPtr<UInvSlotWidget> SlotObj = SlotWidgets.FindRef(SelectedKey))
		{
			SlotObj->MarkSelected(true);
		}
		return;
	}

	if (Inventory->IsInInventory())
	{
		SelectedKey = {};
		if (SlotWidgets.Contains(InKey))
		{
			SelectedKey = InKey;
			for (const TPair<FGuid, TObjectPtr<UInvSlotWidget>>& SlotWidget : SlotWidgets)
			{
				if (!SlotWidget.Value) continue;
				SlotWidget.Value->MarkSelected(SlotWidget.Key == SelectedKey);
			}
		}
	}

	RefreshInfo();
}

void UInventoryWidget::OnViewClicked()
{
	if (Inventory)
	{
		const FInvSlotData& SlotData = Inventory->GetConstInventory()[SelectedKey];
		const UInventoryItemData* ItemData = SlotData.Item.LoadSynchronous();
		if (const FInvItemViewable* Viewable = ItemData->GetFirstAttribute<FInvItemViewable>())
		{
			ViewDescText->SetText(Viewable->Text);
			ViewTitleText->SetText(NameText->GetText());
			ViewDescBox->SetVisibility(Viewable->Text.IsEmptyOrWhitespace() ?
				ESlateVisibility::Collapsed : ESlateVisibility::Visible);

			ViewImage->SetBrushFromTexture(Viewable->Image.LoadSynchronous());
			ViewDescBox->SetVisibility(IsValid(ViewImage->GetBrush().GetResourceObject()) ?
				ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

			if (Viewable->Image.LoadSynchronous() && !Viewable->Text.IsEmptyOrWhitespace())
			{
				ViewDescBox->SetHeightOverride(ImageDescHeight);
			}
			else ViewDescBox->ClearHeightOverride();
			PlayAnimation(ViewFadeAnim);
		}
	}
}

void UInventoryWidget::OnUseClicked()
{
	if (Inventory) Inventory->ConsumeItem(SelectedKey);
}

void UInventoryWidget::OnEquipClicked()
{
	if (Inventory)
	{
		if (EquipmentKey == SelectedKey) Inventory->UnequipItem();
		else Inventory->EquipItem(SelectedKey);
	}
}

void UInventoryWidget::OnFinishViewClicked()
{
	PlayAnimationReverse(ViewFadeAnim);
}

void UInventoryWidget::OnTypeSelected(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	RefreshSlots();
}

void UInventoryWidget::RefreshInfo()
{
	if (!Inventory || !Inventory->IsInInventory()) return;
	if (SelectedKey.IsValid() && SlotWidgets.Contains(SelectedKey) && Inventory->IsValidSlot(SelectedKey))
	{
		const FInvSlotData& SlotData = Inventory->GetConstInventory()[SelectedKey];
		const UInventoryItemData* ItemData = SlotData.Item.LoadSynchronous();
		const bool bEquipped = EquipmentKey == SelectedKey;

		NameText->SetText(ItemData->GetDisplayName(SlotData.Metadata));
		DescText->SetText(ItemData->GetDescription(SlotData.Metadata));
		TypeText->SetText(FText::Format(INVTEXT("Type: {0}"),
			FText::FromString(LexToString(ItemData->ItemType))));

		EquippedBox->SetVisibility(bEquipped ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		MultiUseBox->SetVisibility(SlotData.Metadata.Contains(Tag_InvMeta_ReuseKey)
			? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

		PreviewImage->SetVisibility(Inventory->GetPreviewActor() && Inventory->GetPreviewActor()->SetItem(SelectedKey)
			? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

		if (const FInvItemViewable* Viewable = ItemData->GetFirstAttribute<FInvItemViewable>())
		{
			ViewLabelText->SetText(Viewable->Label);
			ViewButton->SetVisibility(Viewable->IsValidData()
				? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		}
		if (const FInvItemConsumable* Consumable = ItemData->GetFirstAttribute<FInvItemConsumable>())
		{
			if (FIntPoint Amounts; Inventory->CanConsumeItem(SelectedKey, Amounts))
			{
				UseLabelText->SetText(FText::Format(INVTEXT("{0} [{1}/{2}]"),
					Consumable->Label, Amounts.X, Amounts.Y));
			}
			else
			{
				UseLabelText->SetText(Consumable->Label);
			}
			UseButton->SetVisibility(Consumable->IsValidData()
				? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		}
		if (const FInvItemEquipment* Equipment = ItemData->GetFirstAttribute<FInvItemEquipment>())
		{
			EquipLabelText->SetText(bEquipped ? Equipment->UnequipLabel : Equipment->EquipLabel);
			EquipButton->SetVisibility(Equipment->IsValidData()
				? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		}
	}
	else
	{
		PreviewImage->SetVisibility(ESlateVisibility::Collapsed);
		DescText->SetText(NSLOCTEXT("Toro", "NoItemSelected", "No item selected"));
		TypeText->SetText(NSLOCTEXT("Toro", "NoneType", "Type: None"));
		NameText->SetText(NSLOCTEXT("Toro", "None", "None"));
	}

	PlayAnimation(DescFadeAnim);
}

void UInventoryWidget::RefreshSlots()
{
	if (!Inventory) return;
	if (Inventory->IsInInventory())
	{
		EquipmentKey = Inventory->GetEquipmentData().SlotID;
		SlotKeys = Inventory->GetSortedSlots((EInventoryItemType)SlotFilter->GetSelectedIndex());
		const TMap<FGuid, FInvSlotData>& Slots = Inventory->GetConstInventory();

		SlotsBox->ClearChildren();
		SlotWidgets.Empty(SlotKeys.Num());
		if (SlotWidgets.IsEmpty() && SlotKeys.IsEmpty())
		{
			SelectedKey = {};
			RefreshInfo();
			return;
		}

		for (const FGuid& Key : SlotKeys)
		{
			if (UInvSlotWidget* NewSlot = CreateWidget<UInvSlotWidget>(this, SlotWidgetClass))
			{
				NewSlot->ParentUI = this;
				NewSlot->InitData(Key, Slots[Key], Key == EquipmentKey);
				SlotWidgets.Add(Key, NewSlot);
				SlotsBox->AddChild(NewSlot);
			}
		}

		if (SlotKeys.IsEmpty())
		{
			SelectSlot({});
		}
		else if (!SlotKeys.Contains(SelectedKey))
		{
			SelectSlot(SlotKeys[0]);
		}
		else if (const TObjectPtr<UInvSlotWidget> SlotObj = SlotWidgets.FindRef(SelectedKey))
		{
			RefreshInfo();
			SlotObj->MarkSelected(true);
		}

		PlayAnimation(SlotFadeAnim);
	}
}

void UInventoryWidget::InitWidget()
{
	Super::InitWidget();

	UseButton->OnClicked.AddDynamic(this, &ThisClass::OnUseClicked);
	ViewButton->OnClicked.AddDynamic(this, &ThisClass::OnViewClicked);
	EquipButton->OnClicked.AddDynamic(this, &ThisClass::OnEquipClicked);
	FinishViewButton->OnClicked.AddDynamic(this, &ThisClass::OnFinishViewClicked);

	SlotFilter->ClearOptions();
	for (const EInventoryItemType Type : TEnumRange<EInventoryItemType>())
	{
		SlotFilter->AddOption(LexToString(Type));
	}
	SlotFilter->SetSelectedIndex(0);
	SlotFilter->OnSelectionChanged.AddDynamic(this, &ThisClass::OnTypeSelected);

	Inventory = GetPlayerController<AToroPlayerController>()->GetInventory();
	Inventory->OnUpdate.AddUObject(this, &ThisClass::RefreshSlots);
}

void UInventoryWidget::InternalProcessActivation()
{
	Super::InternalProcessActivation();
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		if (!Inventory) Inventory = GetPlayerController<AToroPlayerController>()->GetInventory();
		RefreshSlots();
		RefreshInfo();	
	});
}

void UInventoryWidget::InternalProcessDeactivation()
{
	Super::InternalProcessDeactivation();
	SetAnimationCurrentTime(ViewFadeAnim, ViewFadeAnim->GetStartTime());
}