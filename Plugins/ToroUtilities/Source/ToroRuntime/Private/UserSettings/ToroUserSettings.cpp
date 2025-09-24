// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserSettings/ToroUserSettings.h"

UToroUserSettings::UToroUserSettings(): bInitialized(false)
{
}

bool UToroUserSettings::InitializeSettings()
{
	if (!bInitialized)
	{
		bInitialized = true;
		ApplySettings(false);

		return true;
	}
	return false;
}
