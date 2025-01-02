// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroEditor.h"

#define MAP_TOOL(Tool, ThisClass) \
	Commands->MapAction(FToroEditorCommands::Get().Tool, FExecuteAction::CreateStatic(&ThisClass::ExecuteAction), FCanExecuteAction());

#define REGISTER_TOP_MENU(Tool) \
	if (UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Tools")) \
	{ \
		FToolMenuSection& Section = Menu->FindOrAddSection("ToroEditor", INVTEXT("Toro Utilities")); \
		Section.AddMenuEntry(FToroEditorCommands::Get().Tool); \
	}

#define REGISTER_TOOLBAR(Tool, Category) \
	if (UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar")) \
	{ \
		FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Toolbar."#Category); \
		Section.AddEntry(FToolMenuEntry::InitToolBarButton(FToroEditorCommands::Get().Tool)).SetCommandList(Commands); \
	}

#define LOCTEXT_NAMESPACE "ToroEditor"
class FToroEditorCommands final : public TCommands<FToroEditorCommands>
{
public:

	FToroEditorCommands()
		: TCommands(TEXT("ToroEditor"), LOCTEXT("Contexts", "ToroEditor"),
			NAME_None, FToroEditorStyle::GetStyleSetName())
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