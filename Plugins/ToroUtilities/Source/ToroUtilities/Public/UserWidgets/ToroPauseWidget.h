// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroSettings.h"
#include "Interfaces/ExitInterface.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "ToroPauseWidget.generated.h"

UCLASS(Abstract)
class TOROUTILITIES_API UToroPauseWidget : public UToroWidgetBase, public IExitInterface
{
	GENERATED_BODY()

public:

	UToroPauseWidget(const FObjectInitializer& ObjectInitializer);

protected:

	virtual void ReturnToWidget_Implementation(UUserWidget* FromWidget) override;
	virtual bool CanCreateWidget() const override { return UToroSettings::Get()->IsOnGameplayMap(this); }
};
