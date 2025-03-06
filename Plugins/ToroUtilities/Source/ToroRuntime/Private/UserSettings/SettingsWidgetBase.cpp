// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserSettings/SettingsWidgetBase.h"
#include "GeneralInterface.h"

USettingsWidgetBase::USettingsWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ZOrder = 58;
}

void USettingsWidgetBase::InternalProcessDeactivation()
{
	Super::InternalProcessDeactivation();
	IToroGeneralInterface::Return(ParentUI);
}
