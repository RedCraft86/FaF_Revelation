// Copyright (C) RedCraft86. All Rights Reserved.

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
                "MovieScene",
                "LevelSequence",
                "EngineSettings",
                "AudioExtensions",
                "ApplicationCore",
                "DeveloperSettings",
                "CinematicCamera",
                "MetasoundEngine",
                "GameplayTags",
                "InputCore",
                "CommonUI",
                "ToroCore",
                "DataGraph",
                "Narrative",
                "ExpressiveText",
                "EnhancedCodeFlow", 
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