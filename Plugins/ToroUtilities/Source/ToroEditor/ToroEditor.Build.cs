﻿using UnrealBuildTool;

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
                "Engine",
                "Slate",
                "SlateCore",
                "InputCore",
                "GameplayTags",
                "ContentBrowser",
                "ContentBrowserData",
                "EditorScriptingUtilities",
                "GameplayTagsEditor",
                "DeveloperSettings",
                "EditorSubsystem",
                "AssetDefinition",
                "PropertyEditor",
                "ToolMenus",
                "UnrealEd",
                "Projects",
                "ToroCore",
                "ToroUtilities",
                "ExpressiveText"
            }
        );
    }
}