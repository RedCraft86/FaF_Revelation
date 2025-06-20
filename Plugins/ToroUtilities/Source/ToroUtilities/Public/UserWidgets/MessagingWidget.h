// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroSettings.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "MessagingWidget.generated.h"

class UTextBlock;

USTRUCT(BlueprintType)
struct FToroSimpleMsg
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SimpleMessage)
		FText Message;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SimpleMessage, meta = (ClampMin = 0.0f, UIMin = 0.0f))
		float ExtraTime;

	FToroSimpleMsg(): ExtraTime(0.5f) {}
	FToroSimpleMsg(const FText& InMessage, const float InExtraTime)
		: Message(InMessage), ExtraTime(InExtraTime)
	{}

	bool IsValidMessage() const { return Message.IsEmptyOrWhitespace() || ExtraTime < 0.0f; }
	float CalcTime() const { return ExtraTime + UToroSettings::Get()->GetReadingTime(Message.ToString()); }
};

UCLASS(Abstract)
class TOROUTILITIES_API UMessagingWidget : public UToroWidgetBase
{
	GENERATED_BODY()

public:

	UMessagingWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> NoticeText; // TODO UExprTextBlock

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> TitleText; // TODO UExprTextBlock

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> NoticeAnim;

	UFUNCTION(BlueprintCallable, Category = Message, meta = (WorldContext = "ContextObject"))
		static void QueueNotice(const UObject* ContextObject, const FToroSimpleMsg& InNotice);

	UFUNCTION(BlueprintCallable, Category = Message, meta = (WorldContext = "ContextObject"))
		static void QueueTitle(const UObject* ContextObject, const FToroSimpleMsg& InTitle);

protected:

	FTimerHandle NoticeTimer;
	FTimerHandle TitleTimer;
	TArray<FToroSimpleMsg> Notices;
	TArray<FToroSimpleMsg> Titles;
	
	void NextNotice();
	void ShowNotice(const FToroSimpleMsg& InData);
	void AddNotice(const FToroSimpleMsg& InNotice, const bool bImmediate = false);

	void NextTitle();
	void ShowTitle(const FToroSimpleMsg& InData);
	void AddTitle(const FToroSimpleMsg& InTitle, const bool bImmediate = false);

	virtual void InitWidget() override;
};
