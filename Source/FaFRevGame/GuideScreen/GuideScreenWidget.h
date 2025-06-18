// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroSettings.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "GuideScreenWidget.generated.h"

UCLASS(Abstract)
class FAFREVGAME_API UGuideScreenWidget final : public UToroWidgetBase
{
	GENERATED_BODY()

public:

	UGuideScreenWidget(const FObjectInitializer& ObjectInitializer);

private:

	virtual bool CanCreateWidget() const override { return UToroSettings::Get()->IsOnGameplayMap(this); }
};
