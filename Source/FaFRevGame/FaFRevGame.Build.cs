// Copyright (C) RedCraft86, Inc. All Rights Reserved.

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
                "PhysicsCore",
                "GameplayTags",
                "LevelSequence",
                "AudioExtensions",
                "DeveloperSettings",
                "MetasoundEngine",
                "EnhancedInput",
                "EngineCameras",
                "CommonUI",
                "ToroCore",
                "ToroUtilities",
                "DataGraph",
                "Narrative"
            }
        );
    }
}