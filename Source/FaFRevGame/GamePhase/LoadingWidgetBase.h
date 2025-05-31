// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserWidgets/ToroWidgetBase.h"
#include "LoadingWidgetBase.generated.h"

UCLASS(Abstract)
class FAFREVGAME_API ULoadingWidgetBase final : public UToroWidgetBase
{
	GENERATED_BODY()

public:

	ULoadingWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
	{
		ZOrder = 49;
		bAutoActivate = false;
	}
};
