// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroWidgetBase.h"
#include "PauseWidgetBase.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API UPauseWidgetBase : public UToroWidget
{
	GENERATED_BODY()

public:

	UPauseWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
	{
		ZOrder = 59;
	}
};
