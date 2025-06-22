// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroUtilities.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "ExtrasWidget.generated.h"

UCLASS()
class FAFREVGAME_API UExtrasWidget : public UToroWidgetBase
{
	GENERATED_BODY()

public:

	UExtrasWidget(const FObjectInitializer& ObjectInitializer);

private: // TODO

	virtual bool CanCreateWidget() const override { return UToroUtilSettings::Get()->IsOnLaunchMap(this); }
};
