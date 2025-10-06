﻿// Copyright (C) RedCraft86. All Rights Reserved.

#include "Achievements/AchievementWidget.h"
#include "Achievements/AchievementManager.h"
#include "Animation/UMGSequencePlayer.h"
#include "Components/PanelWidget.h"

void UAchievementEntryWidget::InitializeWidget(const FAchievementEntry& Entry)
{
	TitleText->SetText(Entry.Name);
	IconImage->SetBrushFromTexture(Entry.Icon.LoadSynchronous());
	if (FWidgetAnimationState* State = PlayAnimation(EntryAnim).GetAnimationState())
	{
		State->GetOnWidgetAnimationFinished().AddUObject(
			this, &UAchievementEntryWidget::OnAnimationFinished);
	}
}

UAchievementWidget::UAchievementWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	UUserWidget::SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UAchievementWidget::OnAchievement(const FAchievementEntry& Data)
{
	if (!Data.IsValidData()) return;
	if (UAchievementEntryWidget* Widget = CreateWidget<UAchievementEntryWidget>(this, EntryClass))
	{
		AchievementList->AddChild(Widget);
		Widget->InitializeWidget(Data);
	}
}

void UAchievementWidget::InitWidget(APlayerController* Controller)
{
	Super::InitWidget(Controller);
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		if (UAchievementManager* Manager = UAchievementManager::Get(this))
		{
			Manager->OnAchievement.AddUObject(this, &UAchievementWidget::OnAchievement);
		}
	});
}
