// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserWidgets/ToroWidgetBase.h"
#include "LoadingWidgetBase.generated.h"

UCLASS(Abstract)
class ULoadingWidgetBase : public UToroWidget
{
	GENERATED_BODY()

public:

	ULoadingWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
	{
		ZOrder = 60;
	}
};
