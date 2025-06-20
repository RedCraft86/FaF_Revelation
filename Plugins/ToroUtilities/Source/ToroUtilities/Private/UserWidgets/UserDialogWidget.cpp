// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserWidgets/UserDialogWidget.h"
#include "Framework/ToroWidgetManager.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

UUserDialogWidget::UUserDialogWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ZOrder = 98;
	bAutoActivate = false;
}

void UUserDialogWidget::ShowDialog(const UObject* ContextObject, const FUserDialogSettings& InSettings,
	const FUserDialogTimer& InTimer, const TFunction<void(uint8)>& Callback)
{
	if (!InSettings.IsValidData()) return;
	if (AToroWidgetManager* Manager = AToroWidgetManager::Get(ContextObject))
	{
		if (UUserDialogWidget* Widget = Manager->FindWidget<UUserDialogWidget>())
		{
			Widget->ActivateWidget();
			Widget->InitData(InSettings, InTimer, Callback);
		}
	}
}

void UUserDialogWidget::InitData(const FUserDialogSettings& InSettings, const FUserDialogTimer& InTimer, const TFunction<void(uint8)>& Callback)
{
	TitleText->SetText(InSettings.Title);
	MsgText->SetText(InSettings.Message);
	OptionAText->SetText(InSettings.OptionA);
	OptionBText->SetText(InSettings.OptionB);
	OptionBBtn->SetVisibility(InSettings.OptionB.IsEmptyOrWhitespace()
		? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	
	switch (InTimer.ButtonIdx)
	{
	case 0: AutoOption = InSettings.OptionA; break;
	case 1: AutoOption = InSettings.OptionB; break;
	default: break;
	}
	
	Timer = InTimer;
	CallbackFunc = Callback;
}

void UUserDialogWidget::ButtonAEvent()
{
	if (CallbackFunc) CallbackFunc(0);
	DeactivateWidget();
}

void UUserDialogWidget::ButtonBEvent()
{
	if (CallbackFunc) CallbackFunc(1);
	DeactivateWidget();
}

void UUserDialogWidget::InitWidget()
{
	OptionABtn->OnClicked.AddDynamic(this, &UUserDialogWidget::ButtonAEvent);
	OptionBBtn->OnClicked.AddDynamic(this, &UUserDialogWidget::ButtonBEvent);
}

void UUserDialogWidget::InternalProcessDeactivation()
{
	Timer = {};
	CallbackFunc = nullptr;
	AutoOption = FText::GetEmpty();
	Super::InternalProcessDeactivation();
}

void UUserDialogWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (CallbackFunc && Timer.IsValidData() && !AutoOption.IsEmptyOrWhitespace())
	{
		Timer.WaitTime -= InDeltaTime;
		AutoMsgText->SetText(FText::Format(INVTEXT("Auto picking '{0}' in {1}s"),
			AutoOption, FMath::Max(FMath::RoundToInt32(Timer.WaitTime), 0)));

		if (!Timer.IsValidData())
		{
			switch (Timer.ButtonIdx)
			{
				case 0: ButtonAEvent(); break;
				case 1: ButtonBEvent(); break;
				default: break;
			}
		}
	}
}
