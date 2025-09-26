// Copyright (C) RedCraft86. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class ToroRuntime : ModuleRules
{
    public ToroRuntime(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bUseUnity = false;
        
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public/Gameplay"));
        
        // Fixes a really weird error that honestly makes no sense in this context
        // MovieSceneNumericVariant.h... not enough arguments for function-like macro invocation 'max'...
        PrivateDefinitions.Add("NOMINMAX");

        PublicDependencyModuleNames.AddRange(
            [
                "Core",
                "CoreUObject",
                "Engine",
                "Json",
                "UMG",
                "Slate",
                "SlateCore",
                "EnhancedInput",
                "InputCore",
                "CommonUI",
                "MovieScene",
                "LevelSequence",
                "JsonUtilities",
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