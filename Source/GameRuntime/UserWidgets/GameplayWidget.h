// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "UserWidgets/ToroWidgetBase.h"
#include "GameplayWidget.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType)
class UGameplayWidget final : public UToroActivatableWidget
{
	GENERATED_BODY()

public:

	UGameplayWidget(const FObjectInitializer& ObjectInit);
};
