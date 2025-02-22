// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserWidgets/ToroWidgetBase.h"
#include "GuideBookWidgetBase.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API UGuideBookWidgetBase : public UToroWidget
{
	GENERATED_BODY()

public:

	UGuideBookWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
	{
		ZOrder = 59;
	}
};
