// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserInterface/ToroManagedWidget.h"
#include "GamePauseWidget.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API UGamePauseWidget final : public UToroManagedWidget
{
	GENERATED_BODY()

public:

	UGamePauseWidget(const FObjectInitializer& ObjectInitializer);

protected:

	// TODO impl
};
