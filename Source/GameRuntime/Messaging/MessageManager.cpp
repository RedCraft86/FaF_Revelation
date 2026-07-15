// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "MessageManager.h"
#include "GamePlayer/GamePlayerController.h"

UMessageManager::UMessageManager()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bTickEvenWhenPaused = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.TickGroup = TG_DuringPhysics;
}

UMessageManager* UMessageManager::Get(const UObject* ContextObject)
{
	const AGamePlayerController* PC = AGamePlayerController::Get<AGamePlayerController>(ContextObject);
	return IsValid(PC) ? PC->GetMessageManager() : nullptr;
}

void UMessageManager::QueueNotice(const FGenericMessage& Entry, const EMessageInsertionType InsertionType)
{
	if (!Entry.IsValid())
	{
		return;
	}

	switch (InsertionType)
	{
		case EMessageInsertionType::InsertLast:
			Notices.Emplace(Entry, Entry.GetReadTime());
			break;
		case EMessageInsertionType::InsertNow:
			Notices.EmplaceAt(0, Entry, Entry.GetReadTime());
			break;
		case EMessageInsertionType::Replace:
			if (Notices.IsEmpty())
			{
				Notices.Emplace(Entry, Entry.GetReadTime());
			}
			else
			{
				Notices[0] = { Entry, Entry.GetReadTime() };
			}
			break;
		case EMessageInsertionType::Override:
			Notices.Empty(1);
			Notices.Emplace(Entry, Entry.GetReadTime());
			break;
	}
}

void UMessageManager::QueueNotices(const TArray<FGenericMessage>& Entries, const EMessageInsertionType InsertionType)
{
	bool bIsFirst = false;
	for (const FGenericMessage& Entry : Entries)
	{
		if (Entry.IsValid())
		{
			QueueNotice(Entry, bIsFirst ? InsertionType : EMessageInsertionType::InsertLast);
			bIsFirst = true;
		}
	}
}

void UMessageManager::QueueSubtitle(const FSubtitleMessage& Entry, const EMessageInsertionType InsertionType)
{
	if (!Entry.IsValid())
	{
		return;
	}

	switch (InsertionType)
	{
		case EMessageInsertionType::InsertLast:
			Subtitles.Emplace(Entry, Entry.GetReadTime());
			break;
		case EMessageInsertionType::InsertNow:
			Subtitles.EmplaceAt(0, Entry, Entry.GetReadTime());
			break;
		case EMessageInsertionType::Replace:
			if (Subtitles.IsEmpty())
			{
				Subtitles.Emplace(Entry, Entry.GetReadTime());
			}
			else
			{
				Subtitles[0] = { Entry, Entry.GetReadTime() };
			}
			break;
		case EMessageInsertionType::Override:
			Subtitles.Empty(1);
			Subtitles.Emplace(Entry, Entry.GetReadTime());
			break;
	}
}

void UMessageManager::QueueSubtitles(const TArray<FSubtitleMessage>& Entries, const EMessageInsertionType InsertionType)
{
	bool bIsFirst = false;
	for (const FSubtitleMessage& Entry : Entries)
	{
		if (Entry.IsValid())
		{
			QueueSubtitle(Entry, bIsFirst ? InsertionType : EMessageInsertionType::InsertLast);
			bIsFirst = true;
		}
	}
}

void UMessageManager::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = GetOwner<AGamePlayerController>();
}

void UMessageManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc)
{
	Super::TickComponent(DeltaTime, TickType, TickFunc);

	if (!Notices.IsEmpty())
	{
		if ((Notices[0].Value -= DeltaTime) <= 0.0f)
		{
			Notices.RemoveAt(0);
			if (OnNoticeText.IsBound())
			{
				if (!Notices.IsEmpty())
				{
					OnNoticeText.Execute(Notices[0].Key);
				}
				else
				{
					OnNoticeText.Execute(FGenericMessage());
				}
			}
		}
	}

	if (!Subtitles.IsEmpty() && !PlayerController->IsPaused())
	{
		if ((Subtitles[0].Value -= DeltaTime) <= 0.0f)
		{
			Subtitles.RemoveAt(0);
			if (OnSubtitleText.IsBound())
			{
				if (!Subtitles.IsEmpty())
				{
					OnSubtitleText.Execute(Subtitles[0].Key);
				}
				else
				{
					OnSubtitleText.Execute(FSubtitleMessage());
				}
			}
		}
	}
}

