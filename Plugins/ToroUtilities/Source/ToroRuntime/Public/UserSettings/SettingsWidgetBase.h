// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserWidgets/ToroWidgetBase.h"
#include "SettingsWidgetBase.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API USettingsWidgetBase : public UToroWidget
{
	GENERATED_BODY()

public:

	USettingsWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
	{
		ZOrder = 58;
	}

	DECLARE_MULTICAST_DELEGATE(FOnExit);
	FOnExit OnExit;
};
