﻿// Copyright (C) RedCraft86. All Rights Reserved.

using UnrealBuildTool;

public class ToroGraphEd : ModuleRules
{
    public ToroGraphEd(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

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
                "ToolMenus",
                "GraphEditor",
                "ApplicationCore",
                "AssetDefinition",
                "DeveloperSettings",
                "ToroGraph",
                "ToroCore"
            }
        );
    }
}