// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#define LOCTEXT_NAMESPACE "ToroGraphEd"

class FDataGraphCommands final : public TCommands<FDataGraphCommands>
{
public:

	FDataGraphCommands()
		: TCommands("DataGraphEditor",
			NSLOCTEXT("Contexts", "DataGraphEditor", "Data Graph Editor"),
			NAME_None, FAppStyle::GetAppStyleSetName())
	{}

	virtual void RegisterCommands() override
	{
		UI_COMMAND(AutoArrange, "Auto Arrange", "Auto Arrange", EUserInterfaceActionType::Button, FInputChord());
	}

	TSharedPtr<FUICommandInfo> AutoArrange;
};

#undef LOCTEXT_NAMESPACE