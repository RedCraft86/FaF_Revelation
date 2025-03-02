// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserSettings/SettingsWidgetBase.h"

void USettingsWidgetBase::InternalProcessDeactivation()
{
	Super::InternalProcessDeactivation();
	IToroGeneralInterface::Return(OwnerWidget);
}
