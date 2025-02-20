// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "SettingRowWidgets.generated.h"

UCLASS(Abstract, NotBlueprintable)
class TORORUNTIME_API USettingRowBase : public UUserWidget
{
	GENERATED_BODY()

public:

	USettingRowBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
};

UCLASS(Abstract)
class TORORUNTIME_API UToggleSettingBase : public USettingRowBase
{
	GENERATED_BODY()

public:

	UToggleSettingBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
};

UCLASS(Abstract)
class TORORUNTIME_API USliderSettingBase : public USettingRowBase
{
	GENERATED_BODY()

public:

	USliderSettingBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
};

UCLASS(Abstract)
class TORORUNTIME_API USwitcherSettingBase : public USettingRowBase
{
	GENERATED_BODY()

public:

	USwitcherSettingBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
};

UCLASS(Abstract)
class TORORUNTIME_API UComboSettingBase : public USettingRowBase
{
	GENERATED_BODY()

public:

	UComboSettingBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
};