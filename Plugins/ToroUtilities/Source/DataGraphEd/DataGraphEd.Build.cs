﻿// Copyright (C) RedCraft86. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class DataGraphEd : ModuleRules
{
    public DataGraphEd(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new[]
            {
                Path.Combine(PluginDirectory, "Source/DataGraphEd/Public/AutoLayout"),
                Path.Combine(PluginDirectory, "Source/DataGraphEd/Public/Graph"),
                Path.Combine(PluginDirectory, "Source/DataGraphEd/Public/Nodes")
            }
        );
        
        PrivateDependencyModuleNames.AddRange(
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
                "DataGraph",
                "ToroCore"
            }
        );
    }
}