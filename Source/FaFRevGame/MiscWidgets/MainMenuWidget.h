// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroSettings.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "MainMenuWidget.generated.h"

UCLASS(Abstract)
class FAFREVGAME_API UMainMenuWidget final : public UToroWidgetBase
{
	GENERATED_BODY()

public:

	UMainMenuWidget(const FObjectInitializer& ObjectInitializer);

private:

	virtual bool CanCreateWidget() const override { return UToroSettings::Get()->IsOnLaunchMap(this); }
};
