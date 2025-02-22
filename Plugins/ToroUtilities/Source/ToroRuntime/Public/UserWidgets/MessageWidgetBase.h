// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InputAction.h"
#include "ToroWidgetBase.h"
#include "DataTypes/MessagingTypes.h"
#include "MessageWidgetBase.generated.h"

class UExprTextBlock;

UCLASS(Abstract)
class TORORUNTIME_API UMessageWidgetBase : public UToroWidget
{
	GENERATED_BODY()

public:

	UMessageWidgetBase(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Transient, BlueprintReadOnly,meta = (BindWidget))
		TObjectPtr<UExprTextBlock> SmallNoticeText;

	UPROPERTY(Transient, BlueprintReadOnly,meta = (BindWidget))
		TObjectPtr<UExprTextBlock> LargeNoticeText;
	
	UPROPERTY(Transient, BlueprintReadOnly,meta = (BindWidget))
		TObjectPtr<UExprTextBlock> SubtitleLineText;

	UPROPERTY(Transient, BlueprintReadOnly,meta = (BindWidget))
		TObjectPtr<class UTextBlock> SubtitleNameText;

	UPROPERTY(Transient, BlueprintReadOnly,meta = (BindWidget))
		TObjectPtr<class UHorizontalBox> ControlBar;

	UPROPERTY(Transient, BlueprintReadOnly,meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> SmallNoticeAnim;

	UPROPERTY(Transient, BlueprintReadOnly,meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> LargeNoticeAnim;

	UPROPERTY(Transient, BlueprintReadOnly,meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> SubtitleAnim;

	UPROPERTY(Transient, BlueprintReadOnly,meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> ControlBarAnim;

	UPROPERTY(EditDefaultsOnly, Category = Settings)
		TSubclassOf<class UControlEntryWidgetBase> ControlEntryWidget;

	UPROPERTY(EditDefaultsOnly, Category = Settings)
		FSlateBrush ControlDividerBrush;

	void QueueSmallNotice(const FSimpleMessageData& NoticeData, const bool bResetQueue);
	void QueueLargeNotice(const FSimpleMessageData& NoticeData, const bool bResetQueue);
	void QueueSubtitles(const TArray<FSimpleSubtitleData>& Subtitles, const bool bOverride);
	void QueueSubtitle(const FSimpleSubtitleData& SubtitleData, const bool bOverride);
	void AppendControlEntry(const UInputAction* InAction);
	void RemoveControlEntry(const UInputAction* InAction);

protected:

	UPROPERTY(Transient) TObjectPtr<const AWorldSettings> WorldSettings;
	UPROPERTY(Transient) TObjectPtr<const UInputMappingContext> MappingContext;

	FTimerHandle SmallNoticeTimer;
	TQueue<FSimpleMessageData> SmallNoticeQueue;
	void UpdateSmallNotice();

	FTimerHandle LargeNoticeTimer;
	TQueue<FSimpleMessageData> LargeNoticeQueue;
	void UpdateLargeNotice();

	FTimerHandle SubtitleTimer;
	TQueue<FSimpleSubtitleData> SubtitleQueue;
	void UpdateSubtitle();

	bool bControlBarState;
	TArray<TObjectPtr<const UInputAction>> ControlBarEntries;
	void UpdateControlBar();

	virtual void InitWidget() override;
	virtual bool ShouldBeHidden() override;
};

UCLASS(Abstract)
class TORORUNTIME_API UControlEntryWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UControlEntryWidgetBase(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Transient, meta = (BindWidget))
		TObjectPtr<UTextBlock> LabelText;
	
	UPROPERTY(Transient, meta = (BindWidget))
		TObjectPtr<UHorizontalBox> KeysBox;

	UPROPERTY(EditAnywhere, Category = "Settings")
		FSlateBrush DividerBrush;
	
	UPROPERTY(EditAnywhere, Category = "Settings")
		FVector2D DesiredSize;

	UFUNCTION(BlueprintImplementableEvent)
		struct FRichImageRow GetIconForKey(const FName& InKey);
	
	void SetData(const FText& Name, const TArray<FKey>& Keys);
};