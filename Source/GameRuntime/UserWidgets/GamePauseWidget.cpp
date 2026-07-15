// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "GamePauseWidget.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"

UGamePauseWidget::UGamePauseWidget(const FObjectInitializer& ObjectInit): Super(ObjectInit)
{
}

void UGamePauseWidget::OnContextClicked() const
{
	MenuTitle->SetText(INVTEXT("Paused"));
	MenuSwitcher->SetActiveWidgetIndex(0);
}

void UGamePauseWidget::OnObjectiveClicked() const
{
	MenuTitle->SetText(INVTEXT("Objectives"));
	MenuSwitcher->SetActiveWidgetIndex(1);
}

void UGamePauseWidget::OnArchivesClicked() const
{
	MenuTitle->SetText(INVTEXT("Archives"));
	MenuSwitcher->SetActiveWidgetIndex(2);
}

void UGamePauseWidget::OnInventoryClicked() const
{
	MenuTitle->SetText(INVTEXT("Inventory"));
	MenuSwitcher->SetActiveWidgetIndex(3);
}

void UGamePauseWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ContextButton->OnClicked().AddUObject(this, &UGamePauseWidget::OnContextClicked);
	ObjectiveButton->OnClicked().AddUObject(this, &UGamePauseWidget::OnObjectiveClicked);
	ArchivesButton->OnClicked().AddUObject(this, &UGamePauseWidget::OnArchivesClicked);
	InventoryButton->OnClicked().AddUObject(this, &UGamePauseWidget::OnInventoryClicked);
}
