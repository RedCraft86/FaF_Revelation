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
                "Json",
                "UMG",
                "Slate",
                "SlateCore",
                "MovieScene",
                "LevelSequence",
                "JsonUtilities",
                "EngineSettings",
                "AudioExtensions",
                "ApplicationCore",
                "DeveloperSettings",
                "CinematicCamera",
                "MetasoundEngine",
                "EnhancedInput",
                "GameplayTags",
                "PhysicsCore",
                "InputCore",
                "CommonUI",
                "ToroCore",
                "DataGraph",
                "Narrative",
                "ExpressiveText",
                "EnhancedCodeFlow",
                "XeSSBlueprint",
                "NISBlueprint", 
                "DLSSBlueprint", 
                "StreamlineDLSSGBlueprint",
                "StreamlineReflexBlueprint", 
                "StreamlineDeepDVCBlueprint", 
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