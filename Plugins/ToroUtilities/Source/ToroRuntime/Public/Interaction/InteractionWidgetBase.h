// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserWidgets/ToroWidgetBase.h"
#include "InteractionWidgetBase.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API UInteractionWidgetBase : public UToroWidget
{
	GENERATED_BODY()

public:

	UInteractionWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
	{
		ZOrder = 55;
	}
};
