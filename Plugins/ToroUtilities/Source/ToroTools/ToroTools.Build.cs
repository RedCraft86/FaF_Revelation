// Copyright (C) RedCraft86. All Rights Reserved.

using UnrealBuildTool;

public class ToroTools : ModuleRules
{
    public ToroTools(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateDependencyModuleNames.AddRange(
            new[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "UMG",
                "Slate",
                "SlateCore",
                "InputCore",
                "MainFrame",
                "ToolMenus",
                "UnrealEd",
                "Projects",
                "Blutility",
                "UMGEditor",
                "EngineSettings",
                "EditorScriptingUtilities", 
                "ToroCore"
            }
        );
    }
}