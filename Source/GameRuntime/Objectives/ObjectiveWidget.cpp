// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "ObjectiveWidget.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "Widgets/ExpressiveTextBlock.h"
#include "GameStage/GameStageManager.h"
#include "GameState/GameFlagManager.h"
#include "GameStage/GameStageNode.h"
#include "GameState/SessionState.h"

UObjectiveEntryWidget::UObjectiveEntryWidget(const FObjectInitializer& ObjectInit): Super(ObjectInit)
{
}

void UObjectiveEntryWidget::InitWidget(const FGameplayTag& Key)
{
	if (const FObjectiveDbEntry* Entry = UObjectiveDatabase::GetEntry(Key))
	{
		EntryData = *Entry;
		TitleText->SetText(EntryData.Label);
		SummaryText->SetText(EntryData.ShortText);
	}
}

void UObjectiveEntryWidget::NativeDestruct()
{
	OnHovered.Unbind();
	Super::NativeDestruct();
}

void UObjectiveEntryWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	if (EntryData.IsValid())
	{
		OnHovered.ExecuteIfBound(EntryData);
	}
}

UObjectiveWidget::UObjectiveWidget(const FObjectInitializer& ObjectInit): Super(ObjectInit)
{
}

void UObjectiveWidget::OnEntryHovered(const FObjectiveDbEntry& Data) const
{
	TitleText->SetText(Data.Label);
	DescText->SetText(Data.DetailedText);
}

void UObjectiveWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TitleText->SetText(INVTEXT("None"));
	DescText->SetText(INVTEXT("Hover over an objective to view its description."));

	const ASessionState* Session = ASessionState::Get<ASessionState>(this);
	const UGameFlagManager* FlagManager = Session->GetSessionFlags();
	if (const UGameStageNode* Stage = Session->GetStageManager()->GetActiveStage())
	{
		for (const FGameplayTag& Tag : Stage->GetObjectives())
		{
			if (FlagManager->IsFlagUnlocked(Tag))
			{
				continue;
			}

			if (UObjectiveEntryWidget* EntryWidget = CreateWidget<UObjectiveEntryWidget>(this, EntryWidgetClass))
			{
				EntryWidget->InitWidget(Tag);
				EntryWidget->OnHovered.BindUObject(this, &UObjectiveWidget::OnEntryHovered);
				EntryContainer->AddChild(EntryWidget);
			}
		}
	}
}

void UObjectiveWidget::NativeDestruct()
{
	EntryContainer->ClearChildren();
	Super::NativeDestruct();
}
