// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroToolCommands.h"
#include "Framework/Commands/Commands.h"

class FChannelPacker
{
public:

	static void Register(const TSharedPtr<FUICommandList>& Commands)
	{
		MAP_TOOL(ChannelPacker, FChannelPacker);
	}
	
	static void RegisterMenus(const TSharedPtr<FUICommandList>& Commands)
	{
		REGISTER_TOP_MENU(ChannelPacker)
		REGISTER_TOOLBAR(ChannelPacker, General)
	}

private:

	static void ExecuteAction();
};