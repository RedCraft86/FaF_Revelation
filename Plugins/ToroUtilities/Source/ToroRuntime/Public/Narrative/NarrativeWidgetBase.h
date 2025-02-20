// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserWidgets/ToroWidgetBase.h"
#include "NarrativeWidgetBase.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API UNarrativeWidgetBase : public UToroWidget
{
	GENERATED_BODY()

public:

	UNarrativeWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
	{
		ZOrder = 57;
	}
};
