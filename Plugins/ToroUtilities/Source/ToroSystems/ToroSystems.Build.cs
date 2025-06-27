// Copyright (C) RedCraft86. All Rights Reserved.

using UnrealBuildTool;

public class ToroSystems : ModuleRules
{
    public ToroSystems(ReadOnlyTargetRules Target) : base(Target)
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
                "JsonUtilities",
                "AudioExtensions",
                "MetasoundEngine",
                "DeveloperSettings",
                "GameplayTags",
                "ToroCore",
                "ToroUtilities",
                "ExpressiveText"
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