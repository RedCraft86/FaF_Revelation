// Copyright (C) RedCraft86. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class FaFRevGame : ModuleRules
{
    public FaFRevGame(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bUseUnity = false;

        PublicIncludePaths.AddRange(
                [
                ModuleDirectory, 
                Path.Combine(ModuleDirectory, "Characters")
            ]
        );

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
                "AIModule",
                "MovieScene",
                "LevelSequence",
                "EnhancedInput",
                "EngineCameras",
                "CinematicCamera",
                "NavigationSystem",
                "DeveloperSettings",
                "GameplayTags",
                
                "ToroCore",
                "ToroGraph",
                "ToroRuntime",
                "ExpressiveText",
                "Narrative",
                "SMSystem"
            ]
        );
    }
}