// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "CommonActivatableWidget.h"
#include "ToroWidgetBase.generated.h"

#define InitAnim(Anim) { \
		PlayAnimation(Anim); \
		StopAnimation(Anim); \
	}

UCLASS(Abstract)
class TOROCORE_API UToroWidgetBase : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:

	UToroWidgetBase(const FObjectInitializer& ObjectInitializer);

	// TODO
};
