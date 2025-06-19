// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserWidgets/ToroWidgetBase.h"
#include "GameLoadWidget.generated.h"

// Meant to be subclassed in game modules!
UCLASS(Abstract)
class TOROUTILITIES_API UGameLoadWidget : public UToroWidgetBase
{
	GENERATED_BODY()

public:

	UGameLoadWidget(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
	{
		ZOrder = 98;
		bAutoActivate = false;
	}
};
