﻿// Copyright (C) RedCraft86. All Rights Reserved.

using UnrealBuildTool;

public class ToroRuntime : ModuleRules
{
    public ToroRuntime(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "UMG",
                "Slate",
                "SlateCore",
                "GameplayTags",
                "MovieScene",
                "LevelSequence",
                "EngineSettings",
                "AudioExtensions",
                "ApplicationCore",
                "DeveloperSettings",
                "MetasoundEngine",
                "GameplayTags",
                "CommonUI",
                "ToroCore",
                "DataGraph",
                "ExpressiveText",
                "EnhancedCodeFlow"
            }
        );
        
        if (Target.Type == TargetType.Editor)
        {
            PublicDependencyModuleNames.AddRange(
                new[]
                {
                    "UnrealEd",
                }
            );
        }
    }
}