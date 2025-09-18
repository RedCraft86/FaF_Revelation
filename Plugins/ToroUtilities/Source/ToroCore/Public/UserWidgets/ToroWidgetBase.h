// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "CommonActivatableWidget.h"
#include "ToroWidgetBase.generated.h"

#define InitAnim(Anim) { \
		PlayAnimation(Anim); \
		StopAnimation(Anim); \
	}

#define GetAnimDuration(Anim) (Anim->GetEndTime() - Anim->GetStartTime())

UCLASS(Abstract)
class TOROCORE_API UToroWidgetBase : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:

	UToroWidgetBase(const FObjectInitializer& ObjectInitializer);

	// TODO
};
