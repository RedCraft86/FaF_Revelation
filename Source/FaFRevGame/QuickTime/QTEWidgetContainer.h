// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "QTEManager.h"
#include "UserInterface/ToroContainerWidget.h"
#include "QTEWidgetContainer.generated.h"

UCLASS(Abstract)
class FAFREVGAME_API UQTEWidgetContainer final : public UToroOverlayContainer
{
	GENERATED_BODY()

public:

	UQTEWidgetContainer(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
	{
		ZOrder = 101;
	}

protected:

	TObjectPtr<UQTEManager> QTEManager;

	virtual void InitWidget(APlayerController* Controller) override
	{
		Super::InitWidget(Controller);
		GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
		{
			QTEManager = UQTEManager::Get(this);
		});
	}

	virtual bool ShouldHideWidget() const override
	{
		return !QTEManager || !QTEManager->IsQuicktimeActive();
	}
};
