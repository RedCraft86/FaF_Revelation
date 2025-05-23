﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroEditor.h"

#define MAP_TOOL(Tool, ThisClass) \
	Commands->MapAction(FToroCmds::Get().Tool, FExecuteAction::CreateStatic(&ThisClass::ExecuteAction), FCanExecuteAction());

#define REGISTER_TOP_MENU(Tool) \
	if (UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Tools")) \
	{ \
		FToolMenuSection& Section = Menu->FindOrAddSection("ToroEditor", INVTEXT("Toro Utilities")); \
		Section.AddMenuEntry(FToroCmds::Get().Tool); \
	}

#define REGISTER_TOOLBAR(Tool, Category) \
	if (UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar")) \
	{ \
		FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Toolbar."#Category); \
		Section.AddEntry(FToolMenuEntry::InitToolBarButton(FToroCmds::Get().Tool)).SetCommandList(Commands); \
	}

#define LOCTEXT_NAMESPACE "ToroEditor"
class FToroCmds final : public TCommands<FToroCmds>
{
public:

	FToroCmds()
		: TCommands(TEXT("ToroEditor"), LOCTEXT("Contexts", "ToroEditor"),
			NAME_None, FToroEditorStyle::GetStyleSetName())
	{}

	virtual void RegisterCommands() override
	{
		UI_COMMAND(LinkAssetLibrary, "Link Asset Library", "Links a shared asset library (Specified in ToroEditor settings)",
			EUserInterfaceActionType::Button, FInputChord());

		UI_COMMAND(RestartEditor, "Restart Editor", "Restarts the Unreal Editor",
			EUserInterfaceActionType::Button, FInputChord(EModifierKey::Control | EModifierKey::Shift, EKeys::R));

		UI_COMMAND(ActorLayout, "Actor Layout", "Lay out selected actors in a Overview style",
			EUserInterfaceActionType::Button, FInputChord());

		UI_COMMAND(StaticMeshMerger, "Merge Static Meshes", "Opens the Merge Actors tab",
			EUserInterfaceActionType::Button, FInputChord());

		UI_COMMAND(StaticMeshBaker, "Bake Static Meshes", "Bake static meshes from a single blueprint to individual static meshes",
			EUserInterfaceActionType::Button, FInputChord());

		UI_COMMAND(StaticMeshInstancer, "Instance Static Meshes", "Converts static meshes into Instanced Static Meshes",
			EUserInterfaceActionType::Button, FInputChord());
	}

	TSharedPtr<FUICommandInfo> LinkAssetLibrary;
	TSharedPtr<FUICommandInfo> RestartEditor;
	TSharedPtr<FUICommandInfo> ActorLayout;
	TSharedPtr<FUICommandInfo> StaticMeshMerger;
	TSharedPtr<FUICommandInfo> StaticMeshBaker;
	TSharedPtr<FUICommandInfo> StaticMeshInstancer;
};
#undef LOCTEXT_NAMESPACE