// Copyright (C) RedCraft86. All Rights Reserved.

#include "Narrative/NarrativeWidget.h"

UQuestWidget::UQuestWidget(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	ZOrder = 95;
}

UDialogueWidget::UDialogueWidget(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	ZOrder = 96;
	bAutoActivate = false;
}
