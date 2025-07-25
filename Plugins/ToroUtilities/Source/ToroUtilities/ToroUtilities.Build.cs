﻿// Copyright (C) RedCraft86. All Rights Reserved.

using UnrealBuildTool;

public class ToroUtilities : ModuleRules
{
    public ToroUtilities(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bUseUnity = false;

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
                "InputCore",
                "MovieScene",
                "LevelSequence",
                "JsonUtilities",
                "EngineSettings",
                "ProceduralMeshComponent",
                "DeveloperSettings",
                "EnhancedInput", 
                "GameplayTags",
                "CommonUI",
                "ToroCore",
                "EnhancedCodeFlow",
                "ExpressiveText"
            }
        );
        
        if (Target.Type == TargetType.Editor)
        {
            PublicDependencyModuleNames.AddRange(
                new[]
                {
                    "UnrealEd",
                    "EditorScriptingUtilities"
                }
            );
        }
    }
}