// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserWidgets/ToroWidgetBase.h"
#include "InventoryWidgetBase.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API UInventorySlotWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:

	UInventorySlotWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
};

UCLASS(Abstract)
class TORORUNTIME_API UInventoryWidgetBase : public UToroWidget
{
	GENERATED_BODY()

public:

	UInventoryWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
	{
		ZOrder = 57;
	}

	// TODO
	UPROPERTY(EditAnywhere, Category = Settings)
		TSubclassOf<UInventorySlotWidgetBase> SlotWidget;
};
