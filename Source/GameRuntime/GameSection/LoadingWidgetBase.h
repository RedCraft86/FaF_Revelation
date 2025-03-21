// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroWidget.h"
#include "LoadingWidgetBase.generated.h"

UCLASS(Abstract)
class GAMERUNTIME_API ULoadingWidgetBase : public UToroWidget
{
	GENERATED_BODY()

public:

	ULoadingWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
	{
		ZOrder = 49;
	}
};