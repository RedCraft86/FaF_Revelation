﻿// Copyright (C) RedCraft86. All Rights Reserved.

using System.IO;
using Microsoft.Extensions.Logging;
using UnrealBuildTool;

public class GameRuntime : ModuleRules
{
    public GameRuntime(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.Add(ModuleDirectory);
        PublicDependencyModuleNames.AddRange(
            new[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "UMG",
                "Slate",
                "SlateCore",
                "InputCore",
                "MoviePlayer",
                "PhysicsCore",
                "GameplayTags",
                "LevelSequence",
                "EngineCameras",
                "MetasoundEngine",
                "DeveloperSettings",
                "EnhancedInput",
                "ToroUtilities",
                "ToroCore",
                "DataGraph",
                "CommonUI",
                "EnhancedCodeFlow",
                "Narrative"
            }
        );
    }
}