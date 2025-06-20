// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserWidgets/ToroWidgetBase.h"
#include "UserDialogWidget.generated.h"

class UButton;
class UTextBlock;

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
		TObjectPtr<UButton> OptionABtn;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> OptionAText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> OptionBBtn;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> OptionBText;

	static void ShowDialog(const UObject* ContextObject, const FText& InTitle, const FText& InMessage,
		const FText& InButtonA, const FText& InButtonB, const TFunction<void(uint8)>& Callback);

private:

	TFunction<void(uint8)> CallbackFunc;

	void InitData(const FText& InTitle, const FText& InMessage, const FText& InButtonA,
		const FText& InButtonB, const TFunction<void(uint8)>& Callback);

	UFUNCTION() void ButtonAEvent();
	UFUNCTION() void ButtonBEvent();

	virtual void InitWidget() override;
	virtual void InternalProcessDeactivation() override;
};
