// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserWidgets/CutsceneSkipWidget.h"
#include "MiscActors/ToroSequenceActor.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

UCutsceneSkipWidget::UCutsceneSkipWidget(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
	, SkipKey(EKeys::Enter), HoldTime(2.0f), bHolding(false), Progress(0.0f)
{
	ZOrder = 98;
	bAutoActivate = false;
	UUserWidget::SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UCutsceneSkipWidget::ShowWidget(AToroSequenceActor* InSequence)
{
	if (InSequence)
	{
		Sequence = InSequence;
		ActivateWidget();
	}
}

void UCutsceneSkipWidget::SkipCutscene() const
{
	if (Sequence) Sequence->SkipToEnd();
}

void UCutsceneSkipWidget::SetHoldingState(const bool InState)
{
	if (bHolding != InState)
	{
		bHolding = InState;
		PlayAnimation(HoldAnim, 0.0f, 1,
			bHolding ? EUMGSequencePlayMode::Forward : EUMGSequencePlayMode::Reverse);
	}
}

void UCutsceneSkipWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (SkipKeyText) SkipKeyText->SetText(SkipKey.GetDisplayName());
}

void UCutsceneSkipWidget::InternalProcessDeactivation()
{
	Sequence = nullptr;
	SetHoldingState(false);
	Super::InternalProcessDeactivation();
}

void UCutsceneSkipWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (bHolding && Progress < 1.0f)
	{
		Progress += InDeltaTime / HoldTime;
		if (Progress >= 1.0f)
		{
			Progress = 1.0f;
			SkipCutscene();
		}
	}
	else if (!bHolding && Progress > 0.0f)
	{
		Progress = FMath::Max(Progress - (InDeltaTime * HoldTime), 0.0f);
	}

	SkipProgressBar->SetPercent(Progress);
}

FReply UCutsceneSkipWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (Sequence && InKeyEvent.GetKey() == SkipKey)
	{
		SetHoldingState(true);
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

FReply UCutsceneSkipWidget::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == SkipKey)
	{
		SetHoldingState(false);
		return FReply::Handled();
	}
	return FReply::Unhandled();
}
