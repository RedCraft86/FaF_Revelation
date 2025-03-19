// Copyright (C) RedCraft86. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class ToroEditor : ModuleRules
{
    public ToroEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bUseUnity = true;

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
                "GameplayTagsEditor",
                "ContentBrowserData",
                "EditorScriptingUtilities",
                "DeveloperSettings",
                "ExpressiveText",
                "ToroUtilities",
                "ToroCore"
            }
        );
    }
}