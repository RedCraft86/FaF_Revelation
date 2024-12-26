// Copyright (C) RedCraft86. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class ToroRuntime : ModuleRules
{
    public ToroRuntime(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        
        PublicIncludePaths.AddRange(
            new[]
            {
                Path.Combine(PluginDirectory, "Source/ToroRuntime/Public/Actors"),
                Path.Combine(PluginDirectory, "Source/ToroRuntime/Public/DataTypes"),
                Path.Combine(PluginDirectory, "Source/ToroRuntime/Public/Framework"),
            }
        );

        PublicDependencyModuleNames.AddRange(
            new[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "UMG",
                "Slate",
                "SlateCore",
                "MovieScene",
                "LevelSequence",
                "GameplayTags",
                "StructUtils",
                "GeometryCore",
                "ApplicationCore",
                "NavigationSystem",
                "DeveloperSettings",
                "ProceduralMeshComponent",
                "ToroCore",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore"
            }
        );
    }
}