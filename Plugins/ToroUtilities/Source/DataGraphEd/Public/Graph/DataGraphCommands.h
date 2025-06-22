// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

class FDataGraphCommands final : public TCommands<FDataGraphCommands>
{
public:

	FDataGraphCommands()
		: TCommands("DataGraphEditor",
			NSLOCTEXT("Contexts", "DataGraphEditor", "Data Graph Editor"),
			NAME_None, FAppStyle::GetAppStyleSetName())
	{}

	virtual void RegisterCommands() override;

	TSharedPtr<FUICommandInfo> AutoArrange;
};
