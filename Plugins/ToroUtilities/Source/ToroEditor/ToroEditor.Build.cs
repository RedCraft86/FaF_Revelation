// Copyright (C) RedCraft86. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class ToroEditor : ModuleRules
{
    public ToroEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bUseUnity = true;

        // Include all the immediate folders under .../ToroEditor/Public/
        PublicIncludePaths.AddRange(
            Directory.GetDirectories(Path.Combine(PluginDirectory, "Source/ToroEditor/Public/"))
        );

        PublicDependencyModuleNames.AddRange(
            new[]
            {
                "Core",
                "CoreUObject",
                "UnrealEd",
                "Projects",
                "Engine",
                "Slate",
                "SlateCore",
                "InputCore",
                "ToolMenus",
                "PropertyEditor",
                "ContentBrowser",
                "AssetDefinition",
                "EditorSubsystem",
                "ContentBrowserData",
                "EditorScriptingUtilities",
                "DeveloperSettings",
                "ExpressiveText",
                "ToroCore"
            }
        );
    }
}