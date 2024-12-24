// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroTools.h"

#define MAP_TOOL(Tool, ThisClass) \
	Commands->MapAction(FToroToolCommands::Get().Tool, FExecuteAction::CreateStatic(&ThisClass::ExecuteAction), FCanExecuteAction());

#define REGISTER_TOP_MENU(Tool) \
	if (UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Tools")) \
	{ \
		FToolMenuSection& Section = Menu->FindOrAddSection("ToolsButton.ToroTools", INVTEXT("Toro Utilities")); \
		Section.AddMenuEntry(FToroToolCommands::Get().Tool); \
	}

#define REGISTER_TOOLBAR(Tool, Category) \
	if (UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar")) \
	{ \
		FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("ToolbarButton.ToroTools."#Category); \
		Section.AddEntry(FToolMenuEntry::InitToolBarButton(FToroToolCommands::Get().Tool)).SetCommandList(Commands); \
	}

#define LOCTEXT_NAMESPACE "ToroTools"
class FToroToolCommands final : public TCommands<FToroToolCommands>
{
public:

	FToroToolCommands()
		: TCommands(TEXT("ToroTools"), LOCTEXT("Contexts", "ToroTools"),
			NAME_None, FToroToolsEdStyle::GetStyleSetName())
	{}

	virtual void RegisterCommands() override
	{
		UI_COMMAND(RestartEditor, "Restart Editor", "Restarts the Unreal Editor",
			EUserInterfaceActionType::Button, FInputChord(EModifierKey::Control | EModifierKey::Shift, EKeys::R));
		
		UI_COMMAND(ChannelPacker, "Channel Packer", "Opens the Texture Channel Editor",
			EUserInterfaceActionType::Button, FInputChord());

		UI_COMMAND(StaticMeshMerger, "Merge Static Meshes", "Opens the Merge Actors tab",
			EUserInterfaceActionType::Button, FInputChord());

		UI_COMMAND(StaticMeshBaker, "Bake Static Meshes", "Bake static meshes from a single blueprint to individual static meshes",
			EUserInterfaceActionType::Button, FInputChord());

		UI_COMMAND(StaticMeshInstancer, "Instance Static Meshes", "Converts static meshes into Instanced Static Meshes",
			EUserInterfaceActionType::Button, FInputChord());
	}

	TSharedPtr<FUICommandInfo> RestartEditor;
	TSharedPtr<FUICommandInfo> ChannelPacker;
	TSharedPtr<FUICommandInfo> StaticMeshMerger;
	TSharedPtr<FUICommandInfo> StaticMeshBaker;
	TSharedPtr<FUICommandInfo> StaticMeshInstancer;
};
#undef LOCTEXT_NAMESPACE