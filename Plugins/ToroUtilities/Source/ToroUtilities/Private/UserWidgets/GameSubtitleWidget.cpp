// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserWidgets/GameSubtitleWidget.h"
#include "Framework/ToroWidgetManager.h"
#include "Components/TextBlock.h"
#include "SubtitleManager.h"

UGameSubtitleWidget::UGameSubtitleWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), bCaptureNativeSubtitles(false)
{
	ZOrder = 95;
}

void UGameSubtitleWidget::QueueSubtitle(const UObject* ContextObject, const FToroSubtitle& InSubtitle)
{
	if (AToroWidgetManager* Manager = AToroWidgetManager::Get(ContextObject))
	{
		if (UGameSubtitleWidget* Widget = Manager->FindWidget<UGameSubtitleWidget>())
		{
			Widget->AddSubtitle(InSubtitle);
		}
	}
}

void UGameSubtitleWidget::QueueSubtitles(const UObject* ContextObject, const TArray<FToroSubtitle> InSubtitles)
{
	if (AToroWidgetManager* Manager = AToroWidgetManager::Get(ContextObject))
	{
		if (UGameSubtitleWidget* Widget = Manager->FindWidget<UGameSubtitleWidget>())
		{
			Widget->AddSubtitles(InSubtitles);
		}
	}
}

void UGameSubtitleWidget::AddSubtitle(const FToroSubtitle& InSubtitle)
{
	if (!InSubtitle.IsValidSubtitle()) return;
	if (!Subtitles.IsEmpty() && Subtitles[Subtitles.Num() - 1].Line.EqualTo(InSubtitle.Line)) return;

	Subtitles.Add(InSubtitle);
	if (!GetWorld()->GetTimerManager().IsTimerActive(SubtitleTimer))
	{
		NextSubtitle();
	}
}

void UGameSubtitleWidget::AddSubtitles(const TArray<FToroSubtitle>& InSubtitles)
{
	for (const FToroSubtitle& Subtitle : InSubtitles)
	{
		AddSubtitle(Subtitle);
	}
}

void UGameSubtitleWidget::NextSubtitle()
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

void UGameSubtitleWidget::ShowSubtitle(const FToroSubtitle& InData)
{
	LineText->SetText(InData.Line);
	NameText->SetText(InData.Name.IsEmptyOrWhitespace() ? FText::GetEmpty()
		: FText::Format(INVTEXT("{0}:"), InData.Name));

	PlayAnimation(ActivateAnim);
	GetWorld()->GetTimerManager().SetTimer(SubtitleTimer, this,
		&UGameSubtitleWidget::NextSubtitle, InData.CalcTime(), false);
}

void UGameSubtitleWidget::OnNativeSubtitle(const FText& InText)
{
	GetWorld()->GetTimerManager().ClearTimer(SubtitleTimer);
	ShowSubtitle({FText::GetEmpty(), InText, 0.5f});
}

void UGameSubtitleWidget::InitWidget()
{
	Super::InitWidget();
	LineText->SetText(FText::GetEmpty());
	NameText->SetText(FText::GetEmpty());
	if (bCaptureNativeSubtitles)
	{
		FSubtitleManager::GetSubtitleManager()->OnSetSubtitleText()
			.AddUObject(this, &UGameSubtitleWidget::OnNativeSubtitle);
	}
}
