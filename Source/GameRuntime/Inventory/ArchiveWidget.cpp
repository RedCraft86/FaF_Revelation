// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "ArchiveWidget.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Widgets/ExpressiveTextBlock.h"
#include "InvArchiveDatabase.h"
#include "InventoryManager.h"

UArchiveEntryWidget::UArchiveEntryWidget(const FObjectInitializer& ObjectInit): Super(ObjectInit)
{
}

void UArchiveEntryWidget::OnSelectClicked() const
{
	if (!SelectBorder->IsVisible())
	{
		OnEntrySelected.ExecuteIfBound(this);
	}
}

void UArchiveEntryWidget::InitEntry(const FGameplayTag& Key) const
{
	if (const FInvArchiveDbEntry* Data = UInvArchiveDatabase::GetEntry(Key))
	{
		NameText->SetText(Data->DisplayName);
		IconImage->SetBrushFromTexture(Data->IconImage.LoadSynchronous());
	}
	else
	{
		NameText->SetText(INVTEXT("UNKNOWN"));
	}
}

void UArchiveEntryWidget::SetSelectionState(const bool bSelected) const
{
	SelectBorder->SetVisibility(bSelected ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UArchiveEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SelectButton->OnClicked.AddDynamic(this, &UArchiveEntryWidget::OnSelectClicked);
}

UArchiveWidget::UArchiveWidget(const FObjectInitializer& ObjectInit): Super(ObjectInit)
{
}

void UArchiveWidget::DisplayArchive(const FGameplayTag& Key) const
{
	if (const FInvArchiveDbEntry* Data = UInvArchiveDatabase::GetEntry(Key))
	{
		TitleText->SetText(Data->DisplayName);
		DescText->SetText(Data->Description);
		ContentText->SetText(Data->ContentText);
		if (UTexture2D* Texture = Data->IconImage.LoadSynchronous())
		{
			ContentImage->SetBrushFromTexture(Texture);
			ContentImage->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			ContentImage->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UArchiveWidget::OnEntrySelected(const UArchiveEntryWidget* Entry) const
{
	if (!Entry) return;
	for (const TPair<FGameplayTag, TObjectPtr<UArchiveEntryWidget>>& Pair : Entries)
	{
		const bool bSelected = (Pair.Value == Entry);
		Pair.Value->SetSelectionState(bSelected);
		if (bSelected)
		{
			DisplayArchive(Pair.Key);
		}
	}
}

void UArchiveWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TitleText->SetText(INVTEXT("None"));
	DescText->SetText(INVTEXT("Select an archive entry to view its contents."));
	ContentImage->SetVisibility(ESlateVisibility::Collapsed);
	ContentText->SetText(FText::GetEmpty());

	if (UInventoryManager* Inventory = UInventoryManager::Get(this))
	{
		const TArray<FGameplayTag>& Archives = Inventory->GetArchives();
		for (const FGameplayTag& ArchiveKey : Archives)
		{
			if (UArchiveEntryWidget* EntryWidget = CreateWidget<UArchiveEntryWidget>(this, EntryWidgetClass))
			{
				EntryContainer->AddChild(EntryWidget);
				Entries.Add(ArchiveKey, EntryWidget);
				EntryWidget->InitEntry(ArchiveKey);
			}
		}

		const FGameplayTag InitialSelection = Inventory->GetOpeningArchive();
		OnEntrySelected(Entries.FindRef(InitialSelection));
	}
}

void UArchiveWidget::NativeDestruct()
{
	Entries.Empty();
	EntryContainer->ClearChildren();
	Super::NativeDestruct();
}
