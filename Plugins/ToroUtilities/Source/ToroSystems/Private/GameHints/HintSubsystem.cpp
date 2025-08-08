// Copyright (C) RedCraft86. All Rights Reserved.

#include "GameHints/HintSubsystem.h"
#include "UserWidgets/MessagingWidget.h"
#include "UserWidgets/SubtitleWidget.h"

void UHintSubsystem::QueueHint(const UObject* ContextObject, const FGameplayTag InHint)
{
	if (UHintSubsystem* Subsystem = UHintSubsystem::Get(ContextObject))
	{
		Subsystem->RegisterHint(InHint);
	}
}

void UHintSubsystem::ClearHints()
{
	Hints.Empty();
}

void UHintSubsystem::RegisterHint(const FGameplayTag& InHint)
{
	if (const FHintEntry Entry = UHintDatabase::Get(InHint); Entry.IsValidData())
	{
		Hints.Add(InHint, Entry.GetTime());
	}
}

void UHintSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	for (auto It = Hints.CreateIterator(); It; ++It)
	{
		if (It.Value() > 0.0f)
		{
			It.Value() -= DeltaTime;
		}
		else
		{
			if (const FHintEntry Entry = UHintDatabase::Get(It.Key()); Entry.bUseSubtitle)
			{
				USubtitleWidget::QueueSubtitle(this,
					{INVTEXT("You"), Entry.Message, 2.0f});
			}
			else
			{
				UMessagingWidget::QueueNotice(this, {Entry.Message, 2.0f});
			}
			It.RemoveCurrent();
		}
	}
}
