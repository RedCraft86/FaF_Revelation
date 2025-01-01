// Copyright (C) RedCraft86. All Rights Reserved.

using UnrealBuildTool;

public class ToroEditor : ModuleRules
{
    public ToroEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "InputCore",
                "UnrealEd",
                "Projects",
                "MainFrame",
                "ToolMenus",
                "Blutility",
                "EngineSettings",
                "EditorSubsystem",
                "DeveloperSettings",
                "ComponentVisualizers",
                "PropertyEditor",
                "AssetRegistry",
                "ContentBrowser",
                "ContentBrowserData",
                "EditorScriptingUtilities", 
                "ToroCore",
                "ToroRuntime"
            }
        );
    }
}