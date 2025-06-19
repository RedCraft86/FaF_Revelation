// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserWidgets/GameMsgWidget.h"
#include "Framework/ToroWidgetManager.h"
#include "Components/TextBlock.h"

UGameMsgWidget::UGameMsgWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ZOrder = 99;
}

void UGameMsgWidget::QueueNotice(const UObject* ContextObject, const FToroSimpleMsg& InNotice)
{
	if (AToroWidgetManager* Manager = AToroWidgetManager::Get(ContextObject))
	{
		if (UGameMsgWidget* Widget = Manager->FindWidget<UGameMsgWidget>())
		{
			Widget->AddNotice(InNotice);
		}
	}
}

void UGameMsgWidget::QueueTitle(const UObject* ContextObject, const FToroSimpleMsg& InTitle)
{
	if (AToroWidgetManager* Manager = AToroWidgetManager::Get(ContextObject))
	{
		if (UGameMsgWidget* Widget = Manager->FindWidget<UGameMsgWidget>())
		{
			Widget->AddTitle(InTitle);
		}
	}
}

void UGameMsgWidget::NextNotice()
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

void UGameMsgWidget::ShowNotice(const FToroSimpleMsg& InData)
{
	NoticeText->SetText(InData.Message);
	PlayAnimation(NoticeAnim);

	GetWorld()->GetTimerManager().SetTimer(NoticeTimer, this,
		&UGameMsgWidget::NextNotice, InData.CalcTime(), false);
}

void UGameMsgWidget::AddNotice(const FToroSimpleMsg& InNotice, const bool bImmediate)
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

void UGameMsgWidget::NextTitle()
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

void UGameMsgWidget::ShowTitle(const FToroSimpleMsg& InData)
{
	TitleText->SetText(InData.Message);
	PlayAnimation(ActivateAnim);

	GetWorld()->GetTimerManager().SetTimer(TitleTimer, this,
		&UGameMsgWidget::NextTitle, InData.CalcTime(), false);
}

void UGameMsgWidget::AddTitle(const FToroSimpleMsg& InTitle, const bool bImmediate)
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

void UGameMsgWidget::InitWidget()
{
	Super::InitWidget();
	NoticeText->SetText(FText::GetEmpty());
	TitleText->SetText(FText::GetEmpty());
}
