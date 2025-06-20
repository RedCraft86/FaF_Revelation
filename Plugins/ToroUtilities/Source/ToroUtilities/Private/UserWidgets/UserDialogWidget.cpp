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

void UUserDialogWidget::ShowDialog(const UObject* ContextObject, const FText& InTitle, const FText& InMessage,
	const FText& InButtonA, const FText& InButtonB, const TFunction<void(uint8)>& Callback)
{
	if (InTitle.IsEmptyOrWhitespace() || InMessage.IsEmptyOrWhitespace()
		|| InButtonA.IsEmptyOrWhitespace() || !Callback) return;

	if (AToroWidgetManager* Manager = AToroWidgetManager::Get(ContextObject))
	{
		if (UUserDialogWidget* Widget = Manager->FindWidget<UUserDialogWidget>())
		{
			Widget->ActivateWidget();
			Widget->InitData(InTitle, InMessage, InButtonA, InButtonB, Callback);
		}
	}
}

void UUserDialogWidget::InitData(const FText& InTitle, const FText& InMessage,
	const FText& InButtonA, const FText& InButtonB, const TFunction<void(uint8)>& Callback)
{
	TitleText->SetText(InTitle);
	MsgText->SetText(InMessage);
	OptionAText->SetText(InButtonA);
	OptionBText->SetText(InButtonB);
	OptionBBtn->SetVisibility(InButtonB.IsEmptyOrWhitespace()
		? ESlateVisibility::Collapsed : ESlateVisibility::Visible);

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
	CallbackFunc = nullptr;
	Super::InternalProcessDeactivation();
}
