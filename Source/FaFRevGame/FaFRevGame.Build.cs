﻿// Copyright (C) RedCraft86. All Rights Reserved.

using UnrealBuildTool;

public class FaFRevGame : ModuleRules
{
    public FaFRevGame(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bUseUnity = false;
        
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
                "PhysicsCore",
                "EnhancedInput",
                "LevelSequence",
                "DeveloperSettings",
                "MetasoundEngine",
                "EngineCameras",
                "GameplayTags",
                "MovieScene",
                "CommonUI",
                "ToroCore",
                "ToroGraph",
                "ToroUtilities",
                "ToroSystems",
                "EnhancedCodeFlow",
                "ExpressiveText",
                "Narrative",
                "SMSystem"
            }
        );
    }
}