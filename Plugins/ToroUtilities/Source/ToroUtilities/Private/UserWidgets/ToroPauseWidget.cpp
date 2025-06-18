// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserWidgets/ToroPauseWidget.h"

UToroPauseWidget::UToroPauseWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UToroPauseWidget::ReturnToWidget_Implementation(UUserWidget* FromWidget)
{
	SetHidden(false);
}
