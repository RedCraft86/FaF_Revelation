// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/Interaction/InteractionWidget.h"

UInteractionWidget::UInteractionWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAutoPush = false;
	UUserWidget::SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UInteractionWidget::SetInteractionInfo(const FInteractionInfo& Info) const
{
	IconSwitch->SetActiveWidgetIndex(Info.bEnabled ? 1 : 0);
	InteractText->SetText(Info.Label);
	InteractText->SetVisibility(Info.bEnabled
		? ESlateVisibility::HitTestInvisible
		: ESlateVisibility::Visible);
}
