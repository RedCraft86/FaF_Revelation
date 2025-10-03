// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserInterface/ToroManagedWidget.h"
#include "ToroSettingsWidget.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API UToroSettingsWidget : public UToroManagedWidget
{
	GENERATED_BODY()

public:

	UToroSettingsWidget(const FObjectInitializer& ObjectInitializer);
};
