// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserWidgets/ToroWidgetBase.h"
#include "UserDialogWidget.generated.h"

class UButton;
class UTextBlock;

USTRUCT(BlueprintType)
struct TOROUTILITIES_API FUserDialogSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Settings)
		FText Title;

	UPROPERTY(EditAnywhere, Category = Settings)
		FText Message;

	UPROPERTY(EditAnywhere, Category = Settings)
		FText OptionA;

	UPROPERTY(EditAnywhere, Category = Settings)
		FText OptionB;

	FUserDialogSettings() {}
	FUserDialogSettings(const FText& InTitle, const FText& InMessage, const FText& InOptionA, const FText& InOptionB)
		: Title(InTitle), Message(InMessage), OptionA(InOptionA), OptionB(InOptionB)
	{}

	bool IsValidData() const
	{
		return !(Title.IsEmptyOrWhitespace() || Message.IsEmptyOrWhitespace() || OptionA.IsEmptyOrWhitespace());
	}
};

USTRUCT(BlueprintType)
struct TOROUTILITIES_API FUserDialogTimer
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Timer, meta = (ClampMin = 0.0f, UIMin = 0.0f))
		float WaitTime;

	UPROPERTY(EditAnywhere, Category = Timer)
		uint8 ButtonIdx;

	FUserDialogTimer(): WaitTime(0), ButtonIdx(0) {}
	FUserDialogTimer(const float InTime, const uint8 InButton): WaitTime(InTime), ButtonIdx(InButton) {}
	bool IsValidData() const { return WaitTime > 0.0f; }
};

UCLASS(Abstract)
class TOROUTILITIES_API UUserDialogWidget final : public UToroWidgetBase
{
	GENERATED_BODY()

public:

	UUserDialogWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> TitleText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> MsgText; // TODO UExprTextBlock

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> AutoMsgText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> OptionABtn;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> OptionAText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> OptionBBtn;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> OptionBText;

	static void ShowDialog(const UObject* ContextObject, const FUserDialogSettings& InSettings,
		const FUserDialogTimer& InTimer, const TFunction<void(uint8)>& Callback);

private:

	UPROPERTY() FText AutoOption;
	UPROPERTY() FUserDialogTimer Timer;
	TFunction<void(uint8)> CallbackFunc;

	void InitData(const FUserDialogSettings& InSettings, const FUserDialogTimer& InTimer, const TFunction<void(uint8)>& Callback);

	UFUNCTION() void ButtonAEvent();
	UFUNCTION() void ButtonBEvent();

	virtual void InitWidget() override;
	virtual void InternalProcessDeactivation() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
