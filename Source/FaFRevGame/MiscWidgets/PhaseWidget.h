// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroUtilities.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "PhaseWidget.generated.h"

UCLASS()
class FAFREVGAME_API UPhaseWidget : public UToroWidgetBase
{
	GENERATED_BODY()

public:

	UPhaseWidget(const FObjectInitializer& ObjectInitializer);

private: // TODO

	virtual bool CanCreateWidget() const override { return UToroUtilSettings::Get()->IsOnLaunchMap(this); }
};
