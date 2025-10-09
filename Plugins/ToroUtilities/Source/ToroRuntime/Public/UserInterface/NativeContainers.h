// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserInterface/ToroContainerWidget.h"
#include "NativeContainers.generated.h"

UCLASS()
class TORORUNTIME_API UGameplayWidgetContainer final : public UToroOverlayContainer
{
	GENERATED_BODY()

public:

	UGameplayWidgetContainer(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
	{
		ZOrder = 100;
	}
};

UCLASS()
class TORORUNTIME_API UMenuWidgetContainer final : public UToroStackContainer
{
	GENERATED_BODY()

public:

	UMenuWidgetContainer(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
	{
		ZOrder = 101;
	}
};

UCLASS()
class TORORUNTIME_API UOverlayWidgetContainer final : public UToroOverlayContainer
{
	GENERATED_BODY()

public:

	UOverlayWidgetContainer(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
	{
		ZOrder = 102;
	}
};
