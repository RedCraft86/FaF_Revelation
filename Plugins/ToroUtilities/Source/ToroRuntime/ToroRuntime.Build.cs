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
                "ApplicationCore",
                "GameplayTags",
                "ToroCore",
                "FMODStudio"
            }
        );
    }
}