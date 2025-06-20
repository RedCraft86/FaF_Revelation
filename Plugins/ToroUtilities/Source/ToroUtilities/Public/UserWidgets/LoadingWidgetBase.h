// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserWidgets/ToroWidgetBase.h"
#include "LoadingWidgetBase.generated.h"

// Meant to be subclassed in game modules!
UCLASS(Abstract)
class TOROUTILITIES_API ULoadingWidgetBase : public UToroWidgetBase
{
	GENERATED_BODY()

public:

	ULoadingWidgetBase(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
	{
		ZOrder = 98;
		bAutoActivate = false;
		UUserWidget::SetVisibility(ESlateVisibility::HitTestInvisible);
	}
};
