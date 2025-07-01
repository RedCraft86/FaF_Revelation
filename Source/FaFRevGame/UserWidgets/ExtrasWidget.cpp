// Copyright (C) RedCraft86. All Rights Reserved.

#include "ExtrasWidget.h"
#include "Components/Button.h"
#include "Interfaces/ExitInterface.h"

UExtrasWidget::UExtrasWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ZOrder = 96;
	bAutoActivate = false;
}

void UExtrasWidget::InitWidget()
{
	Super::InitWidget();
	ExitButton->OnClicked.AddDynamic(this, &UExtrasWidget::OnExitClicked);
}

void UExtrasWidget::InternalProcessDeactivation()
{
	Super::InternalProcessDeactivation();
	IExitInterface::ReturnToWidget(ParentUI, this);
}
