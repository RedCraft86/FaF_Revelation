// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroSettings.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "GuideWidget.generated.h"

UCLASS(Abstract)
class FAFREVGAME_API UGuideWidget final : public UToroWidgetBase
{
	GENERATED_BODY()

private:

	virtual bool CanCreateWidget() const override { return UToroSettings::Get()->IsOnGameplayMap(this); }
};
