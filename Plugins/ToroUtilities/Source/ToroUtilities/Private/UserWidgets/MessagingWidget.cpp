// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserWidgets/MessagingWidget.h"
#include "Framework/ToroWidgetManager.h"
#include "Components/TextBlock.h"

UMessagingWidget::UMessagingWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ZOrder = 99;
	UUserWidget::SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UMessagingWidget::QueueNotice(const UObject* ContextObject, const FToroSimpleMsg& InNotice)
{
	if (AToroWidgetManager* Manager = AToroWidgetManager::Get(ContextObject))
	{
		if (UMessagingWidget* Widget = Manager->FindWidget<UMessagingWidget>())
		{
			Widget->AddNotice(InNotice);
		}
	}
}

void UMessagingWidget::QueueTitle(const UObject* ContextObject, const FToroSimpleMsg& InTitle)
{
	if (AToroWidgetManager* Manager = AToroWidgetManager::Get(ContextObject))
	{
		if (UMessagingWidget* Widget = Manager->FindWidget<UMessagingWidget>())
		{
			Widget->AddTitle(InTitle);
		}
	}
}

void UMessagingWidget::NextNotice()
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

void UMessagingWidget::ShowNotice(const FToroSimpleMsg& InData)
{
	NoticeText->SetText(InData.Message);
	PlayAnimation(NoticeAnim);

	GetWorld()->GetTimerManager().SetTimer(NoticeTimer, this,
		&UMessagingWidget::NextNotice, InData.CalcTime(), false);
}

void UMessagingWidget::AddNotice(const FToroSimpleMsg& InNotice, const bool bImmediate)
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

void UMessagingWidget::NextTitle()
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

void UMessagingWidget::ShowTitle(const FToroSimpleMsg& InData)
{
	TitleText->SetText(InData.Message);
	PlayAnimation(ActivateAnim);

	GetWorld()->GetTimerManager().SetTimer(TitleTimer, this,
		&UMessagingWidget::NextTitle, InData.CalcTime(), false);
}

void UMessagingWidget::AddTitle(const FToroSimpleMsg& InTitle, const bool bImmediate)
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

void UMessagingWidget::InitWidget()
{
	Super::InitWidget();
	NoticeText->SetText(FText::GetEmpty());
	TitleText->SetText(FText::GetEmpty());
}
