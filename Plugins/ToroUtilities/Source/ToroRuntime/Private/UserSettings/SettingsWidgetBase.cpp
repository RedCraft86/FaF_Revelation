// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserSettings/SettingsWidgetBase.h"
#include "GeneralInterface.h"

void USettingsWidgetBase::InternalProcessDeactivation()
{
	Super::InternalProcessDeactivation();
	IToroGeneralInterface::Return(ParentUI);
}
