// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserWidgets/ToroSubtitleWidget.h"
#include "Framework/ToroWidgetManager.h"
#include "Components/TextBlock.h"
#include "SubtitleManager.h"

UToroSubtitleWidget::UToroSubtitleWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), bCaptureNativeSubtitles(false)
{
	ZOrder = 95;
}

void UToroSubtitleWidget::QueueSubtitle(const UObject* ContextObject, const FToroSubtitle& InSubtitle, const bool bSmartQueue)
{
	if (AToroWidgetManager* Manager = AToroWidgetManager::Get(ContextObject))
	{
		if (UToroSubtitleWidget* Widget = Manager->FindWidget<UToroSubtitleWidget>())
		{
			Widget->AddSubtitle(InSubtitle, bSmartQueue);
		}
	}
}

void UToroSubtitleWidget::QueueSubtitles(const UObject* ContextObject, const TArray<FToroSubtitle> InSubtitles, const bool bSmartQueue)
{
	if (AToroWidgetManager* Manager = AToroWidgetManager::Get(ContextObject))
	{
		if (UToroSubtitleWidget* Widget = Manager->FindWidget<UToroSubtitleWidget>())
		{
			Widget->AddSubtitles(InSubtitles, bSmartQueue);
		}
	}
}

void UToroSubtitleWidget::AddSubtitle(const FToroSubtitle& InSubtitle, const bool bSmartQueue)
{
	if (!InSubtitle.IsValidSubtitle()) return;
	if (bSmartQueue && !Subtitles.IsEmpty() && Subtitles[Subtitles.Num() - 1].Line.EqualTo(InSubtitle.Line)) return;

	Subtitles.Add(InSubtitle);
	if (!GetWorld()->GetTimerManager().IsTimerActive(SubtitleTimer))
	{
		NextSubtitle();
	}
}

void UToroSubtitleWidget::AddSubtitles(const TArray<FToroSubtitle> InSubtitles, const bool bSmartQueue)
{
	for (const FToroSubtitle& Subtitle : InSubtitles)
	{
		AddSubtitle(Subtitle, bSmartQueue);
	}
}

void UToroSubtitleWidget::NextSubtitle()
{
	if (!Subtitles.IsEmpty())
	{
		ShowSubtitle(Subtitles[0]);
		Subtitles.RemoveAt(0);
	}
	else
	{
		PlayAnimationReverse(ActivateAnim);
	}
}

void UToroSubtitleWidget::ShowSubtitle(const FToroSubtitle& InData)
{
	SetSubtitleText(InData.Name, InData.Line);
	GetWorld()->GetTimerManager().SetTimer(SubtitleTimer, this,
		&UToroSubtitleWidget::NextSubtitle, InData.CalcTime(), false);
}

void UToroSubtitleWidget::OnNativeSubtitle(const FText& InText)
{
	GetWorld()->GetTimerManager().ClearTimer(SubtitleTimer);
	ShowSubtitle({FText::GetEmpty(), InText, 0.5f});
}

void UToroSubtitleWidget::SetSubtitleText(const FText& InName, const FText& InLine)
{
	PlayAnimation(ActivateAnim);
	LineText->SetText(InLine);
	NameText->SetText(FText::Format(INVTEXT("{0}:"), InName));
	NameText->SetVisibility(InName.IsEmptyOrWhitespace() ?
		ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
}

void UToroSubtitleWidget::InitWidget()
{
	Super::InitWidget();
	LineText->SetText(FText::GetEmpty());
	NameText->SetText(FText::GetEmpty());
	if (bCaptureNativeSubtitles)
	{
		FSubtitleManager::GetSubtitleManager()->OnSetSubtitleText()
			.AddUObject(this, &UToroSubtitleWidget::OnNativeSubtitle);
	}
}
