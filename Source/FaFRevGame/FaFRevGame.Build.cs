// Copyright (C) RedCraft86, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FaFRevGame : ModuleRules
{
    public FaFRevGame(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bUseUnity = true;

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
                "CommonUI",
                "GameplayTags",
                "LevelSequence",
                "AudioExtensions",
                "DeveloperSettings",
                "MetasoundEngine",
                "ToroCore",
                "ToroUtilities",
                "DataGraph",
                "Narrative"
            }
        );
    }
}