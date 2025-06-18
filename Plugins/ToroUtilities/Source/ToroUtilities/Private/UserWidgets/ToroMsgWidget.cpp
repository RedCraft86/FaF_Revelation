// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserWidgets/ToroMsgWidget.h"
#include "Framework/ToroWidgetManager.h"
#include "Components/TextBlock.h"

UToroMsgWidget::UToroMsgWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ZOrder = 99;
}

void UToroMsgWidget::QueueNotice(const UObject* ContextObject, const FToroSimpleMsg& InNotice)
{
	if (AToroWidgetManager* Manager = AToroWidgetManager::Get(ContextObject))
	{
		if (UToroMsgWidget* Widget = Manager->FindWidget<UToroMsgWidget>())
		{
			Widget->AddNotice(InNotice);
		}
	}
}

void UToroMsgWidget::QueueTitle(const UObject* ContextObject, const FToroSimpleMsg& InTitle)
{
	if (AToroWidgetManager* Manager = AToroWidgetManager::Get(ContextObject))
	{
		if (UToroMsgWidget* Widget = Manager->FindWidget<UToroMsgWidget>())
		{
			Widget->AddTitle(InTitle);
		}
	}
}

void UToroMsgWidget::NextNotice()
{
	if (!Notices.IsEmpty())
	{
		ShowNotice(Notices[0]);
		Notices.RemoveAt(0);
	}
	else
	{
		PlayAnimationReverse(NoticeAnim);
	}
}

void UToroMsgWidget::ShowNotice(const FToroSimpleMsg& InData)
{
	NoticeText->SetText(InData.Message);
	PlayAnimation(NoticeAnim);

	GetWorld()->GetTimerManager().SetTimer(NoticeTimer, this,
		&UToroMsgWidget::NextNotice, InData.CalcTime(), false);
}

void UToroMsgWidget::AddNotice(const FToroSimpleMsg& InNotice, const bool bImmediate)
{
	if (!InNotice.IsValidMessage()) return;
	if (!Notices.IsEmpty() && Notices[Notices.Num() - 1].Message.EqualTo(InNotice.Message)) return;

	if (bImmediate)
	{
		GetWorld()->GetTimerManager().ClearTimer(NoticeTimer);
		ShowNotice(InNotice);
	}
	else
	{
		Notices.Add(InNotice);
		if (!GetWorld()->GetTimerManager().IsTimerActive(NoticeTimer))
		{
			NextNotice();
		}
	}
}

void UToroMsgWidget::NextTitle()
{
	if (!Titles.IsEmpty())
	{
		ShowNotice(Titles[0]);
		Titles.RemoveAt(0);
	}
	else
	{
		PlayAnimationReverse(ActivateAnim);
	}
}

void UToroMsgWidget::ShowTitle(const FToroSimpleMsg& InData)
{
	TitleText->SetText(InData.Message);
	PlayAnimation(ActivateAnim);

	GetWorld()->GetTimerManager().SetTimer(TitleTimer, this,
		&UToroMsgWidget::NextTitle, InData.CalcTime(), false);
}

void UToroMsgWidget::AddTitle(const FToroSimpleMsg& InTitle, const bool bImmediate)
{
	if (!InTitle.IsValidMessage()) return;
	if (!Titles.IsEmpty() && Titles[Titles.Num() - 1].Message.EqualTo(InTitle.Message)) return;

	if (bImmediate)
	{
		GetWorld()->GetTimerManager().ClearTimer(TitleTimer);
		ShowTitle(InTitle);
	}
	else
	{
		Titles.Add(InTitle);
		if (!GetWorld()->GetTimerManager().IsTimerActive(TitleTimer))
		{
			NextTitle();
		}
	}
}

void UToroMsgWidget::InitWidget()
{
	Super::InitWidget();
	NoticeText->SetText(FText::GetEmpty());
	TitleText->SetText(FText::GetEmpty());
}
