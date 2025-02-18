// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserWidgets/ToroWidgetBase.h"
#include "SectionLoadWidget.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API USectionLoadWidget final : public UToroWidget
{
	GENERATED_BODY()

public:

	USectionLoadWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
	{
		ZOrder = 100;
	}
};
