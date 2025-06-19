// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserWidgets/ToroWidgetBase.h"
#include "GameSettingsWidget.generated.h"

UCLASS(Abstract)
class TOROUTILITIES_API UGameSettingsWidget : public UToroWidgetBase
{
	GENERATED_BODY()

public:

	UGameSettingsWidget(const FObjectInitializer& ObjectInitializer);
};
