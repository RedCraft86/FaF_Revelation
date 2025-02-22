// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserWidgets/ToroWidgetBase.h"
#include "GuideWidgetBase.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API UGuideWidgetBase : public UToroWidget
{
	GENERATED_BODY()

public:

	UGuideWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
	{
		ZOrder = 59;
	}
};
