// Copyright (C) RedCraft86. All Rights Reserved.

#include "MessageWidget.h"
#include "ExprTextBlock.h"
#include "FRGameMode.h"
#include "SubWidgets.h"
#include "NarrativeComponent.h"
#include "Blueprint/WidgetTree.h"
#include "GameFramework/PlayerState.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

UMessageWidgetBase::UMessageWidgetBase(const FObjectInitializer& ObjectInitializer)
	: UGTUserWidget(ObjectInitializer), SmallNoticeText(nullptr), LargeNoticeText(nullptr)
	, SubtitleLineText(nullptr), SubtitleNameText(nullptr), SmallNoticeAnim(nullptr)
	, LargeNoticeAnim(nullptr), SubtitleAnim(nullptr), SubtitlePauseFade(nullptr)
	, bAutoHidden(false), HideCheckTime(0.0f), WorldSettings(nullptr)
{
	ZOrder = 95;
	bAutoAdd = true;

	ControlDividerBrush.TintColor = FLinearColor::Gray;
	ControlDividerBrush.DrawAs = ESlateBrushDrawType::RoundedBox;
	ControlDividerBrush.ImageSize = FVector2D(4.0f, 32.0f);
	ControlDividerBrush.OutlineSettings.CornerRadii = FVector4(2.0f, 2.0f, 2.0f, 2.0f);
	ControlDividerBrush.OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
	ControlDividerBrush.OutlineSettings.bUseBrushTransparency = true;
}

void UMessageWidgetBase::QueueSmallNotice(const FSimpleNoticeData& NoticeData, const bool bResetQueue)
{
	if (!NoticeData.IsValidData()) return;
	if (!SmallNoticeQueue.IsEmpty() && GetTypeHash(*SmallNoticeQueue.Peek()) == GetTypeHash(NoticeData)) return;
	if (bResetQueue)
	{
		GetWorld()->GetTimerManager().ClearTimer(SmallNoticeTimer);
		SmallNoticeQueue.Empty();
	}

	SmallNoticeQueue.Enqueue(NoticeData);
	if (!SmallNoticeTimer.IsValid() || !GetWorld()->GetTimerManager().IsTimerActive(SmallNoticeTimer))
	{
		UpdateSmallNotice();
	}
}

void UMessageWidgetBase::QueueLargeNotice(const FSimpleNoticeData& NoticeData, const bool bResetQueue)
{
	if (!NoticeData.IsValidData()) return;
	if (!LargeNoticeQueue.IsEmpty() && GetTypeHash(*LargeNoticeQueue.Peek()) == GetTypeHash(NoticeData)) return;
	if (bResetQueue)
	{
		GetWorld()->GetTimerManager().ClearTimer(LargeNoticeTimer);
		LargeNoticeQueue.Empty();
	}

	LargeNoticeQueue.Enqueue(NoticeData);
	if (!LargeNoticeTimer.IsValid() || !GetWorld()->GetTimerManager().IsTimerActive(LargeNoticeTimer))
	{
		UpdateLargeNotice();
	}
}

void UMessageWidgetBase::QueueSubtitles(const TArray<FSimpleSubtitleData>& Subtitles, const bool bOverride)
{
	int i = 0;
	for (const FSimpleSubtitleData& Subtitle : Subtitles)
	{
		QueueSubtitle(Subtitle, i == 0 && bOverride);
		i++;
	}
}

void UMessageWidgetBase::QueueSubtitle(const FSimpleSubtitleData& SubtitleData, const bool bOverride)
{
	if (!SubtitleData.IsValidData()) return;

	if (bOverride)
	{
		SubtitleQueue.Empty();
		GetWorld()->GetTimerManager().ClearTimer(SubtitleTimer);
	}
	
	SubtitleQueue.Enqueue(SubtitleData);
	if (!SubtitleTimer.IsValid() || !GetWorld()->GetTimerManager().IsTimerActive(SubtitleTimer))
	{
		UpdateSubtitle();
	}
}

void UMessageWidgetBase::AddControlEntry(const FControlKeyData& InData)
{
	ControlBarEntries.RemoveAll([InData](const FControlKeyData& Element) -> bool
	{
		return Element.ID == InData.ID;
	});
	ControlBarEntries.Add(InData);
	UpdateControlBar();
}

void UMessageWidgetBase::RemoveControlEntry(const FName& InID)
{
	ControlBarEntries.RemoveAll([InID](const FControlKeyData& Element) -> bool
	{
		return Element.ID == InID;
	});
	
	UpdateControlBar();
}

void UMessageWidgetBase::UpdateSmallNotice()
{
	FSimpleNoticeData Data;
	if (SmallNoticeQueue.Dequeue(Data))
	{
		if (Data.IsValidData())
		{
			PlayAnimation(SmallNoticeAnim);
			SmallNoticeText->SetText(Data.Message, true);
			GetWorld()->GetTimerManager().SetTimer(SmallNoticeTimer,
				this, &UMessageWidgetBase::UpdateSmallNotice,
				Data.CalcDisplayTime(), false);
		}
		else
		{
			UpdateSmallNotice();
		}
	}
	else
	{
		PlayAnimationReverse(SmallNoticeAnim);
		SmallNoticeTimer.Invalidate();
	}
}

void UMessageWidgetBase::UpdateLargeNotice()
{
	FSimpleNoticeData Data;
	if (LargeNoticeQueue.Dequeue(Data))
	{
		if (Data.IsValidData())
		{
			PlayAnimation(LargeNoticeAnim);
			LargeNoticeText->SetText(Data.Message, true);
			GetWorld()->GetTimerManager().SetTimer(LargeNoticeTimer,
				this, &UMessageWidgetBase::UpdateLargeNotice,
				Data.CalcDisplayTime(), false);
		}
		else
		{
			UpdateLargeNotice();
		}
	}
	else
	{
		PlayAnimationReverse(LargeNoticeAnim);
		LargeNoticeTimer.Invalidate();
	}
}

void UMessageWidgetBase::UpdateSubtitle()
{
	FSimpleSubtitleData Data;
	if (SubtitleQueue.Dequeue(Data))
	{
		if (Data.IsValidData())
		{
			PlayAnimation(SubtitleAnim);
			SubtitleNameText->SetText(Data.Speaker);
			SubtitleLineText->SetText(Data.Line, true);
			GetWorld()->GetTimerManager().SetTimer(SubtitleTimer,
				this, &UMessageWidgetBase::UpdateSubtitle,
				Data.CalcDisplayTime(), false);
		}
		else
		{
			UpdateSubtitle();
		}
	}
	else
	{
		PlayAnimationReverse(SubtitleAnim);
		SubtitleTimer.Invalidate();
	}
}

void UMessageWidgetBase::UpdateControlBar()
{
	ControlBar->ClearChildren();
	if (!ControlEntryClass) return;
	const int32 EntryNum = ControlBarEntries.Num();
	for (int32 i = 0; i < EntryNum; i++)
	{
		UFRControlBarEntry* Entry = WidgetTree->ConstructWidget<UFRControlBarEntry>(ControlEntryClass);
		Entry->SetControlKeyData(ControlBarEntries[i]);
		ControlBar->AddChild(Entry);

		if (EntryNum != 1 && i != EntryNum - 1)
		{
			UImage* DividerImage = WidgetTree->ConstructWidget<UImage>();
			DividerImage->SetBrush(ControlDividerBrush);
						
			UHorizontalBoxSlot* DividerSlot = Cast<UHorizontalBoxSlot>(ControlBar->AddChild(DividerImage));
			DividerSlot->SetPadding(FMargin(10.0f, 0.0f));
			DividerSlot->SetHorizontalAlignment(HAlign_Center);
			DividerSlot->SetVerticalAlignment(VAlign_Fill);
		}
	}

	if (const bool bNewState = !ControlBarEntries.IsEmpty(); bControlBarState != bNewState)
	{
		bControlBarState = bNewState;
		bControlBarState ? PlayAnimationForward(ControlBarAnim) : PlayAnimationReverse(ControlBarAnim);
	}
}

void UMessageWidgetBase::HideCheck()
{
	HideCheckTime = 0.0f;
	if (!WorldSettings || IsAnimationPlaying(SubtitlePauseFade)) return;
	const bool bNewAutoHidden = IsValid(WorldSettings->GetPauserPlayerState()) || GetGameMode<AFRGameModeBase>()->Narrative->IsInDialogue();
	if (bAutoHidden != bNewAutoHidden)
	{
		bAutoHidden = bNewAutoHidden;
		bAutoHidden ? PlayAnimationForward(SubtitlePauseFade) : PlayAnimationReverse(SubtitlePauseFade);
	}
}

void UMessageWidgetBase::InitWidget()
{
	WorldSettings = GetWorld()->GetWorldSettings();
}

void UMessageWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (HideCheckTime >= 0.25f) { HideCheck(); }
	else { HideCheckTime += InDeltaTime; }
}
