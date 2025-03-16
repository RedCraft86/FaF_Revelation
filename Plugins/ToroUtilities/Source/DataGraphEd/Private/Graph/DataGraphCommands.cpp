// Copyright (C) RedCraft86. All Rights Reserved.

#include "DataGraphCommands.h"

#define LOCTEXT_NAMESPACE "DataGraphEditor"

void FDataGraphCommands::RegisterCommands()
{
	UI_COMMAND(AutoArrange, "Auto Arrange", "Auto Arrange", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
