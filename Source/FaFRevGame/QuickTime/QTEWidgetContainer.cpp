// Copyright (C) RedCraft86. All Rights Reserved.

#include "QTEWidgetContainer.h"
#include "QTEManager.h"

UQTEWidgetContainer::UQTEWidgetContainer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ZOrder = 101;
}

void UQTEWidgetContainer::InitWidget(APlayerController* Controller)
{
	Super::InitWidget(Controller);
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		QTEManager = UQTEManager::Get(this);
	});
}

bool UQTEWidgetContainer::ShouldHideWidget() const
{
	return !QTEManager || !QTEManager->IsQuicktimeActive();
}
