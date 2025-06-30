// Copyright (C) RedCraft86. All Rights Reserved.

using UnrealBuildTool;

public class ToroSystems : ModuleRules
{
    public ToroSystems(ReadOnlyTargetRules Target) : base(Target)
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
                "JsonUtilities",
                "AudioExtensions",
                "MetasoundEngine",
                "DeveloperSettings",
                "GameplayTags",
                "CommonUI",
                "ToroCore",
                "ToroGraph",
                "ToroUtilities",
                "EnhancedCodeFlow",
                "ExpressiveText",
                "Narrative"
            }
        );
        
        if (Target.Type == TargetType.Editor)
        {
            PublicDependencyModuleNames.AddRange(
                new[]
                {
                    "UnrealEd"
                }
            );
        }
    }
}