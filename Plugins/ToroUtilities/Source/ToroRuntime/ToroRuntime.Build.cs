// Copyright (C) RedCraft86. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class ToroRuntime : ModuleRules
{
    public ToroRuntime(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public/Gameplay"));

        PublicDependencyModuleNames.AddRange(
            [
                "Core",
                "CoreUObject",
                "Engine",
                "UMG",
                "Slate",
                "SlateCore",
                "EnhancedInput",
                "InputCore",
                "CommonUI",
                "MovieScene",
                "LevelSequence",
                "GameplayTags",
                "EngineSettings",
                "MetasoundEngine",
                "AudioExtensions",
                "DeveloperSettings",
                
                "ToroCore",
                "Narrative",
                "EnhancedCodeFlow",
                "ExpressiveText"
            ]
        );
        
        if (Target.Type == TargetType.Editor)
        {
            PrivateDependencyModuleNames.AddRange(
                [
                    "UnrealEd"
                ]
            );
        }
    }
}