// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroWidgetBase.h"
#include "InfoWidgetBase.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API UInfoWidgetBase : public UToroWidget
{
	GENERATED_BODY()

public:

	UInfoWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
	{
		ZOrder = 56;
	}
};
