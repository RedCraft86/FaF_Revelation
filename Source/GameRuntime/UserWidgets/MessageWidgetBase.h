// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroWidget.h"
#include "InputAction.h"
#include "DataTypes/MessagingTypes.h"
#include "PlayerChar/GamePlayerBase.h"
#include "MessageWidgetBase.generated.h"

class UExprTextBlock;

UCLASS(Abstract)
class GAMERUNTIME_API UMessageWidgetBase : public UToroWidget
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
		TSubclassOf<class UControlEntryBase> ControlEntryWidget;

	UPROPERTY(EditDefaultsOnly, Category = Settings)
		FSlateBrush ControlDividerBrush;

	void AddSmallNotice(const FSimpleMessageData& NoticeData, const bool bResetQueue);
	void AddLargeNotice(const FSimpleMessageData& NoticeData, const bool bResetQueue);
	void AddSubtitles(const TArray<FSimpleSubtitleData>& Subtitles, const bool bOverride);
	void AddSubtitle(const FSimpleSubtitleData& SubtitleData, const bool bOverride);
	void AppendControl(const UInputAction* InAction);
	void RemoveControl(const UInputAction* InAction);

protected:

	UPROPERTY(Transient) TObjectPtr<AGamePlayerBase> Player;
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
	virtual bool CanCreateWidget() const override;

public:

	UFUNCTION(BlueprintCallable, Category = Messaging, meta = (WorldContext = "ContextObject"))
		static void QueueSmallNotice(const UObject* ContextObject, const FSimpleMessageData& NoticeData, const bool bResetQueue);

	UFUNCTION(BlueprintCallable, Category = Guide, meta = (WorldContext = "ContextObject"))
		static void QueueLargeNotice(const UObject* ContextObject, const FSimpleMessageData& NoticeData, const bool bResetQueue);

	UFUNCTION(BlueprintCallable, Category = Guide, meta = (WorldContext = "ContextObject"))
		static void QueueSubtitles(const UObject* ContextObject, const TArray<FSimpleSubtitleData>& Subtitles, const bool bOverride);

	UFUNCTION(BlueprintCallable, Category = Guide, meta = (WorldContext = "ContextObject"))
		static void QueueSubtitle(const UObject* ContextObject, const FSimpleSubtitleData& SubtitleData, const bool bOverride);

	UFUNCTION(BlueprintCallable, Category = Guide, meta = (WorldContext = "ContextObject"))
		static void AppendControlEntry(const UObject* ContextObject, const UInputAction* InAction);

	UFUNCTION(BlueprintCallable, Category = Guide, meta = (WorldContext = "ContextObject"))
		static void RemoveControlEntry(const UObject* ContextObject, const UInputAction* InAction);
};

UCLASS(Abstract)
class GAMERUNTIME_API UControlEntryBase : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UControlEntryBase(const FObjectInitializer& ObjectInitializer);

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