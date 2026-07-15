// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "DialogueWidget.h"
#include "MessageManager.h"
#include "Components/TextBlock.h"
#include "Widgets/ExpressiveTextBlock.h"

UDialogWidget::UDialogWidget(const FObjectInitializer& ObjectInit): Super(ObjectInit)
{
}

void UDialogWidget::OnSubtitleMessage(const FSubtitleMessage& InSubtitle)
{
	if (InSubtitle.IsValid())
	{
		SpeakerLabel->SetText(InSubtitle.Speaker);
		SubtitleText->SetText(InSubtitle.Content);
		PlayAnimationForward(FadeAnim);
	}
	else
	{
		PlayAnimationReverse(FadeAnim);
	}
}

void UDialogWidget::NativeConstruct()
{
	Super::NativeConstruct();
	GetOrAddAnimationState(FadeAnim);
	if (UMessageManager* Manager = UMessageManager::Get(this))
	{
		Manager->OnSubtitleText.BindUObject(this, &UDialogWidget::OnSubtitleMessage);
	}
}

void UDialogWidget::NativeDestruct()
{
	UMessageManager* Manager = UMessageManager::Get(this);
	if (Manager && Manager->OnSubtitleText.GetUObject() == this)
	{
		Manager->OnSubtitleText.Unbind();
	}
	Super::NativeDestruct();
}
