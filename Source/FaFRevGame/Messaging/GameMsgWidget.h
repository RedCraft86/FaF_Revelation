// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserWidgets/ToroWidgetBase.h"
#include "GameMsgWidget.generated.h"

UCLASS(Abstract)
class FAFREVGAME_API UGameMsgWidget final : public UToroWidgetBase
{
	GENERATED_BODY()

public:

	UGameMsgWidget(const FObjectInitializer& ObjectInitializer);
};
