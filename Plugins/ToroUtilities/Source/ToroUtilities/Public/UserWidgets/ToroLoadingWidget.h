// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserWidgets/ToroWidgetBase.h"
#include "ToroLoadingWidget.generated.h"

// Meant to be subclassed in game modules!
UCLASS(Abstract)
class TOROUTILITIES_API UToroLoadingWidget : public UToroWidgetBase
{
	GENERATED_BODY()

public:

	UToroLoadingWidget(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
	{
		ZOrder = 98;
		bAutoActivate = false;
	}
};
