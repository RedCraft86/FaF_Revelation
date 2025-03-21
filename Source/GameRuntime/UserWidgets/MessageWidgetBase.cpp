// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserWidgets/MessageWidgetBase.h"
#include "Achievements/AchievementManager.h"
#include "Components/RichTextBlockImageDecorator.h"
#include "Framework/ToroWidgetManager.h"
#include "UserWidgets/ExprTextBlock.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/HorizontalBox.h"
#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"
#include "Components/ScaleBox.h"
#include "Components/SizeBox.h"
#include "Components/Image.h"
#include "InputMappingContext.h"

UMessageWidgetBase::UMessageWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), bControlBarState(false), AchievementFormat(INVTEXT("[25pt](Achievement Unlocked)\n{0}"))
{
	ZOrder = 45;
	ControlDividerBrush.TintColor = FLinearColor::Gray;
	ControlDividerBrush.DrawAs = ESlateBrushDrawType::RoundedBox;
	ControlDividerBrush.ImageSize = FVector2D(4.0f, 32.0f);
	ControlDividerBrush.OutlineSettings.CornerRadii = FVector4(2.0f, 2.0f, 2.0f, 2.0f);
	ControlDividerBrush.OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
	ControlDividerBrush.OutlineSettings.bUseBrushTransparency = true;
}

void UMessageWidgetBase::AddSmallNotice(const FSimpleMessageData& NoticeData, const bool bResetQueue)
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

void UMessageWidgetBase::AddLargeNotice(const FSimpleMessageData& NoticeData, const bool bResetQueue)
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

void UMessageWidgetBase::AddSubtitles(const TArray<FSimpleSubtitleData>& Subtitles, const bool bOverride)
{
	int i = 0;
	for (const FSimpleSubtitleData& Subtitle : Subtitles)
	{
		AddSubtitle(Subtitle, i == 0 && bOverride);
		i++;
	}
}

void UMessageWidgetBase::AddSubtitle(const FSimpleSubtitleData& SubtitleData, const bool bOverride)
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

void UMessageWidgetBase::AppendControl(const UInputAction* InAction)
{
	ControlBarEntries.Remove(InAction);
	ControlBarEntries.Add(InAction);
	UpdateControlBar();
}

void UMessageWidgetBase::RemoveControl(const UInputAction* InAction)
{
	ControlBarEntries.Remove(InAction);
	UpdateControlBar();
}

void UMessageWidgetBase::OnAchievement(const FAchievementEntry& Data, const uint8 Progress)
{
	if (!AchievementFormat.IsEmptyOrWhitespace() && Data.Requirement == Progress)
	{
		AddLargeNotice({FText::Format(AchievementFormat, Data.Name), 0.0f}, false);
	}
}

void UMessageWidgetBase::UpdateSmallNotice()
{
	if (FSimpleMessageData Data; SmallNoticeQueue.Dequeue(Data))
	{
		if (Data.IsValidData())
		{
			PlayAnimation(SmallNoticeAnim);
			SmallNoticeText->SetText(Data.Message, true);
			GetWorld()->GetTimerManager().SetTimer(SmallNoticeTimer,
				this, &UMessageWidgetBase::UpdateSmallNotice,
				Data.GetDuration(), false);
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
	if (FSimpleMessageData Data; LargeNoticeQueue.Dequeue(Data))
	{
		if (Data.IsValidData())
		{
			PlayAnimation(LargeNoticeAnim);
			LargeNoticeText->SetText(Data.Message, true);
			GetWorld()->GetTimerManager().SetTimer(LargeNoticeTimer,
				this, &UMessageWidgetBase::UpdateLargeNotice,
				Data.GetDuration(), false);
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
	
	if (FSimpleSubtitleData Data; SubtitleQueue.Dequeue(Data))
	{
		if (Data.IsValidData())
		{
			PlayAnimation(SubtitleAnim);
			SubtitleNameText->SetText(Data.Speaker);
			SubtitleLineText->SetText(Data.Message, true);
			GetWorld()->GetTimerManager().SetTimer(SubtitleTimer,
				this, &UMessageWidgetBase::UpdateSubtitle,
				Data.GetDuration(), false);
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
	if (!ControlEntryClass || !MappingContext) return;
	const int32 EntryNum = ControlBarEntries.Num();
	for (int32 i = 0; i < EntryNum; i++)
	{
		if (const UInputAction* Action = ControlBarEntries[i])
		{
			TArray<FKey> Keys;
			const TArray<FEnhancedActionKeyMapping>& Mappings = MappingContext->GetMappings();
			for (const FEnhancedActionKeyMapping& Mapping : Mappings)
			{
				if (Mapping.Action == Action)
				{
					Keys.Add(Mapping.Key);
				}
			}
			
			UControlEntryBase* Entry = WidgetTree->ConstructWidget<UControlEntryBase>(ControlEntryClass);
			Entry->SetData(Action->ActionDescription, Keys);
			ControlBar->AddChild(Entry);

			if (EntryNum != 1 && i != EntryNum - 1)
			{
				UImage* DividerImage = WidgetTree->ConstructWidget<UImage>();
				DividerImage->SetBrush(ControlDividerBrush);
						
				UHorizontalBoxSlot* DividerSlot = Cast<UHorizontalBoxSlot>(ControlBar->AddChild(DividerImage));
				DividerSlot->SetPadding(FMargin(25.0f, 0.0f));
				DividerSlot->SetHorizontalAlignment(HAlign_Center);
				DividerSlot->SetVerticalAlignment(VAlign_Fill);
			}
		}
	}
	
	if (const bool bNewState = !ControlBarEntries.IsEmpty(); bControlBarState != bNewState)
	{
		bControlBarState = bNewState;
		bControlBarState ? PlayAnimationForward(ControlBarAnim) : PlayAnimationReverse(ControlBarAnim);
	}
}

void UMessageWidgetBase::InitWidget()
{
	Super::InitWidget();
	Player = AToroPlayerCharacter::Get<AGamePlayerBase>(this);
	MappingContext = UToroSettings::Get()->DefaultInputMappings.LoadSynchronous();
	if (UAchievementManager* Manager = UAchievementManager::Get(this))
	{
		Manager->OnAchievement.BindUObject(this, &UMessageWidgetBase::OnAchievement);
	}
}

bool UMessageWidgetBase::ShouldBeHidden()
{
	return Super::ShouldBeHidden() || GetOwningPlayer()->IsPaused() || (Player && Player->IsInDialogue());
}

bool UMessageWidgetBase::CanCreateWidget() const
{
	return UToroSettings::Get()->IsOnGameplayMap(this);
}

void UMessageWidgetBase::QueueSmallNotice(const UObject* ContextObject, const FSimpleMessageData& NoticeData, const bool bResetQueue)
{
	AToroWidgetManager* Manager = AToroWidgetManager::Get(ContextObject);
	if (UMessageWidgetBase* Widget = Manager ? Manager->FindWidget<UMessageWidgetBase>() : nullptr)
	{
		Widget->AddSmallNotice(NoticeData, bResetQueue);
	}
}

void UMessageWidgetBase::QueueLargeNotice(const UObject* ContextObject, const FSimpleMessageData& NoticeData, const bool bResetQueue)
{
	AToroWidgetManager* Manager = AToroWidgetManager::Get(ContextObject);
	if (UMessageWidgetBase* Widget = Manager ? Manager->FindWidget<UMessageWidgetBase>() : nullptr)
	{
		Widget->AddLargeNotice(NoticeData, bResetQueue);
	}
}

void UMessageWidgetBase::QueueSubtitles(const UObject* ContextObject, const TArray<FSimpleSubtitleData>& Subtitles, const bool bOverride)
{
	AToroWidgetManager* Manager = AToroWidgetManager::Get(ContextObject);
	if (UMessageWidgetBase* Widget = Manager ? Manager->FindWidget<UMessageWidgetBase>() : nullptr)
	{
		Widget->AddSubtitles(Subtitles, bOverride);
	}
}

void UMessageWidgetBase::QueueSubtitle(const UObject* ContextObject, const FSimpleSubtitleData& SubtitleData, const bool bOverride)
{
	AToroWidgetManager* Manager = AToroWidgetManager::Get(ContextObject);
	if (UMessageWidgetBase* Widget = Manager ? Manager->FindWidget<UMessageWidgetBase>() : nullptr)
	{
		Widget->AddSubtitle(SubtitleData, bOverride);
	}
}

void UMessageWidgetBase::AppendControlEntry(const UObject* ContextObject, const UInputAction* InAction)
{
	AToroWidgetManager* Manager = AToroWidgetManager::Get(ContextObject);
	if (UMessageWidgetBase* Widget = Manager ? Manager->FindWidget<UMessageWidgetBase>() : nullptr)
	{
		Widget->AppendControl(InAction);
	}
}

void UMessageWidgetBase::RemoveControlEntry(const UObject* ContextObject, const UInputAction* InAction)
{
	AToroWidgetManager* Manager = AToroWidgetManager::Get(ContextObject);
	if (UMessageWidgetBase* Widget = Manager ? Manager->FindWidget<UMessageWidgetBase>() : nullptr)
	{
		Widget->RemoveControl(InAction);
	}
}

UControlEntryBase::UControlEntryBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), DesiredSize({0.0f, 50.0f})
{
	DividerBrush.TintColor = FLinearColor::Gray;
	DividerBrush.DrawAs = ESlateBrushDrawType::RoundedBox;
	DividerBrush.ImageSize = FVector2D(4.0f, 32.0f);
	DividerBrush.OutlineSettings.CornerRadii = FVector4(2.0f, 2.0f, 2.0f, 2.0f);
	DividerBrush.OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
	DividerBrush.OutlineSettings.bUseBrushTransparency = true;
}

void UControlEntryBase::SetData(const FText& Name, const TArray<FKey>& Keys)
{
	LabelText->SetText(Name);

	FSlateBrush Brush = {};
	const int32 KeyNum = Keys.Num();
	for (int32 i = 0; i < KeyNum; i++)
	{
		UImage* IconImage = WidgetTree->ConstructWidget<UImage>();
		Brush = GetIconForKey(Keys[i].GetFName()).Brush;
		IconImage->SetBrush(Brush);

		UScaleBox* ScaleBox = WidgetTree->ConstructWidget<UScaleBox>();
		ScaleBox->SetStretch(EStretch::ScaleToFitY);
		ScaleBox->AddChild(IconImage);
		
		USizeBox* SizeBox = WidgetTree->ConstructWidget<USizeBox>();
		DesiredSize.X > 0.0f ? SizeBox->SetWidthOverride(DesiredSize.X) : SizeBox->ClearWidthOverride();
		SizeBox->SetHeightOverride(DesiredSize.Y);
		SizeBox->AddChild(ScaleBox);
		
		UHorizontalBoxSlot* IconSlot = Cast<UHorizontalBoxSlot>(KeysBox->AddChild(SizeBox));
		IconSlot->SetHorizontalAlignment(HAlign_Left);
		IconSlot->SetVerticalAlignment(VAlign_Center);

		// if (KeyNum != 1 && i != KeyNum - 1)
		// {
		// 	UImage* DividerImage = WidgetTree->ConstructWidget<UImage>();
		// 	DividerImage->SetBrush(DividerBrush);
		// 				
		// 	UHorizontalBoxSlot* DividerSlot = Cast<UHorizontalBoxSlot>(KeysBox->AddChild(DividerImage));
		// 	DividerSlot->SetPadding(FMargin(5.0f, 0.0f));
		// 	DividerSlot->SetHorizontalAlignment(HAlign_Center);
		// 	DividerSlot->SetVerticalAlignment(VAlign_Fill);
		// }
	}
}