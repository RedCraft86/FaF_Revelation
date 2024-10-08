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

	UMessageWidgetBase(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Transient, meta = (BindWidget))
		TObjectPtr<UExprTextBlock> SmallNoticeText;

	UPROPERTY(Transient, meta = (BindWidget))
		TObjectPtr<UExprTextBlock> LargeNoticeText;
	
	UPROPERTY(Transient, meta = (BindWidget))
		TObjectPtr<UExprTextBlock> SubtitleLineText;

	UPROPERTY(Transient, meta = (BindWidget))
		TObjectPtr<class UTextBlock> SubtitleNameText;

	UPROPERTY(Transient, meta = (BindWidget))
		TObjectPtr<class UHorizontalBox> ControlBar;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> SmallNoticeAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> LargeNoticeAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> SubtitleAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> ControlBarAnim;
	
	UPROPERTY(Transient, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> SubtitlePauseFade;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		TSubclassOf<class UFRControlBarEntry> ControlEntryClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		FSlateBrush ControlDividerBrush;
	
	void QueueSmallNotice(const FSimpleNoticeData& NoticeData, const bool bResetQueue = false);
	void QueueLargeNotice(const FSimpleNoticeData& NoticeData, const bool bResetQueue = false);
	void QueueSubtitles(const TArray<FSimpleSubtitleData>& Subtitles, const bool bOverride = false);
	void QueueSubtitle(const FSimpleSubtitleData& SubtitleData, const bool bOverride = false);
	void AddControlEntry(const FControlKeyData& InData);
	void RemoveControlEntry(const FName& InID);
	
protected:

	bool bAutoHidden;
	float HideCheckTime;
	UPROPERTY(Transient) TObjectPtr<const AWorldSettings> WorldSettings;

	FTimerHandle SmallNoticeTimer;
	TQueue<FSimpleNoticeData> SmallNoticeQueue;
	void UpdateSmallNotice();

	FTimerHandle LargeNoticeTimer;
	TQueue<FSimpleNoticeData> LargeNoticeQueue;
	void UpdateLargeNotice();

	FTimerHandle SubtitleTimer;
	TQueue<FSimpleSubtitleData> SubtitleQueue;
	void UpdateSubtitle();

	bool bControlBarState = false;
	TArray<FControlKeyData> ControlBarEntries;
	void UpdateControlBar();

	void HideCheck();
	virtual void InitWidget() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
