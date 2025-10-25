// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserInterface/ToroContainerWidget.h"
#include "QTEWidgetContainer.generated.h"

UCLASS(Abstract)
class FAFREVGAME_API UQTEWidgetContainer final : public UToroOverlayContainer
{
	GENERATED_BODY()

public:

	UQTEWidgetContainer(const FObjectInitializer& ObjectInitializer);

protected:

	TObjectPtr<class UQTEManager> QTEManager;

	virtual void InitWidget(APlayerController* Controller) override;
	virtual bool ShouldHideWidget() const override;
};
