// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserInterface/Elements/ToroButtonList.h"
#include "Components/VerticalBoxSlot.h"
#include "Blueprint/WidgetTree.h"

void UToroButtonEntry::SelectButton(bool bImmediate)
{
	if (!bSelected)
	{
		bSelected = true;
		PlayAnimationForward(SelectAnim, bImmediate ? 10.0f : 1.0f);
	}
}

void UToroButtonEntry::DeselectButton(bool bImmediate)
{
	if (bSelected)
	{
		bSelected = false;
		PlayAnimationReverse(SelectAnim, bImmediate ? 10.0f : 1.0f);
	}
}

void UToroButtonEntry::SetDisplayData(const FText& InText, const FSlateFontInfo& InFont) const
{
	Label->SetText(InText);
	Label->SetFont(InFont);
}

void UToroButtonEntry::OnButtonClicked()
{
	if (OnClicked.IsBound())
	{
		SelectButton();
		OnClicked.Execute(this);
	}
}

void UToroButtonEntry::NativeConstruct()
{
	Super::NativeConstruct();
	Button->OnClicked.AddUniqueDynamic(this, &UToroButtonEntry::OnButtonClicked);
}

UToroButtonList::UToroButtonList(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer), EntryPadding(0.0f, 2.0f), SelectedIndex(INDEX_NONE)
{
#if WITH_EDITOR
	const ConstructorHelpers::FObjectFinder<UObject> FontFinder(TEXT("/Engine/EngineFonts/Roboto.Roboto"));
	if (FontFinder.Succeeded())
	{
		EntryFont = FSlateFontInfo(FontFinder.Object, 24, "Regular");
	}
#endif
}

void UToroButtonList::OnSelectionMade(UToroButtonEntry* Button)
{
	for (int i = 0; i < Buttons.Num(); i++)
	{
		if (Buttons[i] == Button)
		{
			SelectedIndex = i;
			OnSelection.Broadcast(i);
			OnSelectionBP.Broadcast(i);
		}
		else
		{
			Buttons[i]->DeselectButton();
		}
	}
}

void UToroButtonList::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (!WidgetTree || !Container)
		return;

	Container->ClearChildren();
	Buttons.Empty(Entries.Num());
	for (const FText& Entry : Entries)
	{
		if (Entry.IsEmptyOrWhitespace()) continue;
		if (UToroButtonEntry* Button = WidgetTree->ConstructWidget<UToroButtonEntry>(EntryClass))
		{
			Buttons.Add(Button);
			Button->SetDisplayData(Entry, EntryFont);
			if (UVerticalBoxSlot* VSlot = Cast<UVerticalBoxSlot>(Container->AddChild(Button)))
			{
				VSlot->SetSize(EntrySize);
				VSlot->SetPadding(EntryPadding);
			}
#if WITH_EDITOR
			if (IsDesignTime())
				continue;
#endif
			Button->OnClicked.BindUObject(this, &UToroButtonList::OnSelectionMade);
			if (Buttons.Num() == 1)
			{
				SelectedIndex = 0;
				Button->SelectButton(true); // Select first
			}
		}
	}
}
