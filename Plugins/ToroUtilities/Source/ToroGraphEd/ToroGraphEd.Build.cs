// Copyright (C) RedCraft86. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class ToroGraphEd : ModuleRules
{
    public ToroGraphEd(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new[]
            {
                Path.Combine(PluginDirectory, "Source/ToroGraphEd/Public/AutoLayout"),
                Path.Combine(PluginDirectory, "Source/ToroGraphEd/Public/Graph"),
                Path.Combine(PluginDirectory, "Source/ToroGraphEd/Public/Nodes")
            }
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