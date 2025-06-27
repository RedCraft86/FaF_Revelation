// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserWidgets/ToroWidgetBase.h"
#include "InventoryWidget.generated.h"

UCLASS(Abstract)
class TOROSYSTEMS_API UInventoryWidget final : public UToroWidgetBase
{
	GENERATED_BODY()

public:

	UInventoryWidget(const FObjectInitializer& ObjectInitializer);
};
