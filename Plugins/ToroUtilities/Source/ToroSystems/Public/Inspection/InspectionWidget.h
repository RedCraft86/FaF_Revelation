// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserWidgets/ToroWidgetBase.h"
#include "InspectionWidget.generated.h"

UCLASS(Abstract)
class TOROSYSTEMS_API UInspectionWidget final : public UToroWidgetBase
{
	GENERATED_BODY()

public:

	UInspectionWidget(const FObjectInitializer& ObjectInitializer);
};
