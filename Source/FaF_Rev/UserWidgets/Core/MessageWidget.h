﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GTUserWidget.h"
#include "MessagingData.h"
#include "MessageWidget.generated.h"

class UExprTextBlock;

UCLASS(Abstract)
class FAF_REV_API UMessageWidgetBase final : public UGTUserWidget
{
	GENERATED_BODY()

public:

	void QueueSmallNotice(const FSimpleNoticeData& NoticeData, const bool bResetQueue = false);
	void QueueLargeNotice(const FSimpleNoticeData& NoticeData, const bool bResetQueue = false);
	void QueueSubtitles(const TArray<FSimpleSubtitleData>& Subtitles);
	void QueueSubtitle(const FSimpleSubtitleData& SubtitleData);
	void SetSubtitlesHidden(const bool bHidden);

private:
	
	UPROPERTY(Transient, meta = (BindWidget))
		UExprTextBlock* SmallNoticeText;

	UPROPERTY(Transient, meta = (BindWidget))
		UExprTextBlock* LargeNoticeText;
	
	UPROPERTY(Transient, meta = (BindWidget))
		UExprTextBlock* SubtitleLineText;

	UPROPERTY(Transient, meta = (BindWidget))
		class UTextBlock* SubtitleNameText;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* SmallNoticeAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* LargeNoticeAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* SubtitleAnim;
	
	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* SubtitleHideAnim;

	FTimerHandle SmallNoticeTimer;
	TQueue<FSimpleNoticeData> SmallNoticeQueue;
	void UpdateSmallNotice();

	FTimerHandle LargeNoticeTimer;
	TQueue<FSimpleNoticeData> LargeNoticeQueue;
	void UpdateLargeNotice();

	FTimerHandle SubtitleTimer;
	TQueue<FSimpleSubtitleData> SubtitleQueue;
	void UpdateSubtitle();
};
