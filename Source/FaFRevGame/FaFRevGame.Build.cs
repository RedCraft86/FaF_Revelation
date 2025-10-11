// Copyright (C) RedCraft86. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class FaFRevGame : ModuleRules
{
    public FaFRevGame(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Characters"));

        PublicDependencyModuleNames.AddRange(
            [
                "Core",
                "CoreUObject", 
                "Engine",
                "UMG",
                "Slate",
                "SlateCore",
                "CommonUI",
                "PhysicsCore",
                "InputCore",
                "MovieScene",
                "LevelSequence",
                "EnhancedInput",
                "EngineCameras",
                "CinematicCamera",
                "DeveloperSettings",
                "GameplayTags",
                
                "ToroCore",
                "ToroGraph",
                "ToroRuntime",
                "EnhancedCodeFlow"
            ]
        );
    }
}