// Copyright (C) RedCraft86. All Rights Reserved.

#include "Achievements/AchievementWidget.h"
#include "Achievements/AchievementManager.h"
#include "Animation/UMGSequencePlayer.h"
#include "Components/PanelWidget.h"

UAchievementEntryWidget* UAchievementEntryWidget::CreateEntry(UUserWidget* Owner, const FAchievementEntry& Entry)
{
	if (!Entry.IsValidData()) return nullptr;
	if (UAchievementEntryWidget* Widget = CreateWidget<UAchievementEntryWidget>(Owner))
	{
		Widget->InitializeWidget(Entry);
		return Widget;
	}
	return nullptr;
}

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

void UAchievementWidget::OnAchievement(const FAchievementEntry& Data)
{
	AchievementList->AddChild(UAchievementEntryWidget::CreateEntry(this, Data));
}

void UAchievementWidget::NativeConstruct()
{
	Super::NativeConstruct();
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		if (UAchievementManager* Manager = UAchievementManager::Get(this))
		{
			Manager->OnAchievement.AddUObject(this, &UAchievementWidget::OnAchievement);
		}
	});
}
