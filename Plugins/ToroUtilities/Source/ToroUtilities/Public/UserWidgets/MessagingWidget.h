// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroUtilities.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "Components/RichTextBlockImageDecorator.h"
#include "MessagingWidget.generated.h"

class UTextBlock;
class UExprTextBlock;

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

	bool IsValidData() const { return !Message.IsEmptyOrWhitespace() && ExtraTime >= 0.0f; }
	float CalcTime() const { return ExtraTime + UToroUtilSettings::Get()->GetReadingTime(Message.ToString()); }
};

USTRUCT(BlueprintType)
struct FToroInputPrompt
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Prompt)
		FText Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Prompt)
		TSet<FKey> Keys;

	FToroInputPrompt() {}
	FToroInputPrompt(const FText& InName, const TSet<FKey>& InKeys): Name(InName), Keys(InKeys) {}
	bool IsValidData() const { return !Name.IsEmptyOrWhitespace() && !Keys.IsEmpty(); }
};

UCLASS(Abstract)
class TOROUTILITIES_API UInputPreviewEntry final : public UUserWidget
{
	GENERATED_BODY()

public:

	UInputPreviewEntry(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer) {}

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> NameText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UPanelWidget> KeysBox;

	UFUNCTION(BlueprintImplementableEvent)
		FRichImageRow GetKeyIcon(const FName& InKey);

	void InitData(const FToroInputPrompt& InPreview);
};

UCLASS(Abstract)
class TOROUTILITIES_API UMessagingWidget final : public UToroWidgetBase
{
	GENERATED_BODY()

public:

	UMessagingWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UExprTextBlock> NoticeText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UExprTextBlock> TitleText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> NoticeAnim;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> TitleAnim;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> InputRowAnim;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UPanelWidget> InputBox;

	UPROPERTY(EditAnywhere, Category = Settings)
		TSubclassOf<UInputPreviewEntry> InputEntryWidget;

	UFUNCTION(BlueprintCallable, Category = Message, meta = (WorldContext = "ContextObject"))
		static void QueueNotice(const UObject* ContextObject, const FToroSimpleMsg& InNotice, bool bImmediate = false);

	UFUNCTION(BlueprintCallable, Category = Message, meta = (WorldContext = "ContextObject"))
		static void QueueTitle(const UObject* ContextObject, const FToroSimpleMsg& InTitle);

	UFUNCTION(BlueprintCallable, Category = Message, meta = (WorldContext = "ContextObject"))
		static void ShowInputPrompt(const UObject* ContextObject, const FName InKey, const FToroInputPrompt& InData);

	UFUNCTION(BlueprintCallable, Category = Message, meta = (WorldContext = "ContextObject"))
		static void HideInputPrompt(const UObject* ContextObject, const FName InKey);

	UFUNCTION(BlueprintCallable, Category = Message, meta = (WorldContext = "ContextObject"))
		static void ClearInputPrompts(const UObject* ContextObject);

protected:

	UPROPERTY() FTimerHandle NoticeTimer;
	UPROPERTY() FTimerHandle TitleTimer;
	UPROPERTY() TArray<FToroSimpleMsg> Notices;
	UPROPERTY() TArray<FToroSimpleMsg> Titles;
	UPROPERTY(Transient) TMap<FName, TObjectPtr<UInputPreviewEntry>> InputPreviews;
	
	void NextNotice();
	void ShowNotice(const FToroSimpleMsg& InData);
	void AddNotice(const FToroSimpleMsg& InNotice, const bool bImmediate = false);

	void NextTitle();
	void ShowTitle(const FToroSimpleMsg& InData);
	void AddTitle(const FToroSimpleMsg& InTitle, const bool bImmediate = false);

	void ClearInputRows();
	void RemoveInputRow(const FName& InKey);
	void AddInputRow(const FName& InKey, const FToroInputPrompt& InData);

	virtual void InitWidget() override;
	virtual void NativeConstruct() override;
};
