// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserWidgets/SubtitleWidget.h"
#include "Framework/ToroWidgetManager.h"
#include "Components/TextBlock.h"
#include "SubtitleManager.h"

USubtitleWidget::USubtitleWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), bCaptureNativeSubtitles(false)
{
	ZOrder = 95;
	UUserWidget::SetVisibility(ESlateVisibility::HitTestInvisible);
}

void USubtitleWidget::QueueSubtitle(const UObject* ContextObject, const FToroSubtitle& InSubtitle)
{
	if (AToroWidgetManager* Manager = AToroWidgetManager::Get(ContextObject))
	{
		if (USubtitleWidget* Widget = Manager->FindWidget<USubtitleWidget>())
		{
			Widget->AddSubtitle(InSubtitle);
		}
	}
}

void USubtitleWidget::QueueSubtitles(const UObject* ContextObject, const TArray<FToroSubtitle> InSubtitles)
{
	if (AToroWidgetManager* Manager = AToroWidgetManager::Get(ContextObject))
	{
		if (USubtitleWidget* Widget = Manager->FindWidget<USubtitleWidget>())
		{
			Widget->AddSubtitles(InSubtitles);
		}
	}
}

void USubtitleWidget::AddSubtitle(const FToroSubtitle& InSubtitle)
{
	if (!InSubtitle.IsValidSubtitle()) return;
	if (!Subtitles.IsEmpty() && Subtitles[Subtitles.Num() - 1].Line.EqualTo(InSubtitle.Line)) return;

	Subtitles.Add(InSubtitle);
	if (!GetWorld()->GetTimerManager().IsTimerActive(SubtitleTimer))
	{
		NextSubtitle();
	}
}

void USubtitleWidget::AddSubtitles(const TArray<FToroSubtitle>& InSubtitles)
{
	for (const FToroSubtitle& Subtitle : InSubtitles)
	{
		AddSubtitle(Subtitle);
	}
}

void USubtitleWidget::NextSubtitle()
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

void USubtitleWidget::ShowSubtitle(const FToroSubtitle& InData)
{
	LineText->SetText(InData.Line);
	NameText->SetText(InData.Name.IsEmptyOrWhitespace() ? FText::GetEmpty()
		: FText::Format(INVTEXT("{0}:"), InData.Name));

	PlayAnimation(ActivateAnim);
	GetWorld()->GetTimerManager().SetTimer(SubtitleTimer, this,
		&USubtitleWidget::NextSubtitle, InData.CalcTime(), false);
}

void USubtitleWidget::OnNativeSubtitle(const FText& InText)
{
	GetWorld()->GetTimerManager().ClearTimer(SubtitleTimer);
	ShowSubtitle({FText::GetEmpty(), InText, 0.5f});
}

void USubtitleWidget::InitWidget()
{
	Super::InitWidget();
	LineText->SetText(FText::GetEmpty());
	NameText->SetText(FText::GetEmpty());
	if (bCaptureNativeSubtitles)
	{
		FSubtitleManager::GetSubtitleManager()->OnSetSubtitleText()
			.AddUObject(this, &USubtitleWidget::OnNativeSubtitle);
	}
}
