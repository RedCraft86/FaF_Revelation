// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "InventoryWidget.h"
#include "InventoryManager.h"
#include "InvItemDatabase.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Buttons/CommonLabeledButton.h"
#include "Widgets/ExpressiveTextBlock.h"

UInventoryEntryWidget::UInventoryEntryWidget(const FObjectInitializer& ObjectInit): Super(ObjectInit)
{
}

void UInventoryEntryWidget::OnSelectClicked() const
{
	if (!SelectBorder->IsVisible())
	{
		OnEntrySelected.ExecuteIfBound(this);
	}
}

void UInventoryEntryWidget::InitEntry(const FGameplayTag& Key) const
{
	if (const FInvItemDbEntry* Data = UInvItemDatabase::GetEntry(Key))
	{
		NameText->SetText(Data->DisplayName);
		IconImage->SetBrushFromTexture(Data->IconImage.LoadSynchronous());
	}
	else
	{
		NameText->SetText(INVTEXT("UNKNOWN"));
	}
}

void UInventoryEntryWidget::SetSelectionState(const bool bSelected) const
{
	SelectBorder->SetVisibility(bSelected ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UInventoryEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SelectButton->OnClicked.AddDynamic(this, &UInventoryEntryWidget::OnSelectClicked);
}

UInventoryWidget::UInventoryWidget(const FObjectInitializer& ObjectInit): Super(ObjectInit)
{
}

void UInventoryWidget::DisplayItem(const FGameplayTag& Key) const
{
	if (const FInvItemDbEntry* Data = UInvItemDatabase::GetEntry(Key))
	{
		TitleText->SetText(Data->DisplayName);
		DescText->SetText(Data->Description);

		EquipButton->SetVisibility(Key.MatchesTag(TAG_InvEquipment.GetTag()) 
			? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

		EquipButton->SetLabelContentText((Inventory->GetEquipment() == Key) 
			? INVTEXT("Unequip") : INVTEXT("Equip"));
	}
}

void UInventoryWidget::OnEntrySelected(const UInventoryEntryWidget* Entry)
{
	if (!Entry) return;
	for (const TPair<FGameplayTag, TObjectPtr<UInventoryEntryWidget>>& Pair : Entries)
	{
		const bool bSelected = (Pair.Value == Entry);
		Pair.Value->SetSelectionState(bSelected);
		if (bSelected)
		{
			Selection = Pair.Key;
			DisplayItem(Selection);
		}
	}
}

void UInventoryWidget::OnEquipClicked() const
{
	if (Selection.IsValid())
	{
		if (Inventory->GetEquipment() == Selection)
		{
			Inventory->UnequipItem();
		}
		else
		{
			Inventory->EquipItem(Selection);
		}

		DisplayItem(Selection);
	}
}

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TitleText->SetText(INVTEXT("None"));
	DescText->SetText(INVTEXT("Select an item entry to equip it (if possible) or view its description."));
	EquipButton->SetVisibility(ESlateVisibility::Collapsed);

	if (Inventory = UInventoryManager::Get(this); Inventory.IsValid())
	{
		const TArray<FGameplayTag>& Items = Inventory->GetItems();
		for (const FGameplayTag& ItemKey : Items)
		{
			if (UInventoryEntryWidget* EntryWidget = CreateWidget<UInventoryEntryWidget>(this, EntryWidgetClass))
			{
				EntryContainer->AddChild(EntryWidget);
				Entries.Add(ItemKey, EntryWidget);
				EntryWidget->InitEntry(ItemKey);
			}
		}
	}
}

void UInventoryWidget::NativeDestruct()
{
	Entries.Empty();
	EntryContainer->ClearChildren();
	Super::NativeDestruct();
}
