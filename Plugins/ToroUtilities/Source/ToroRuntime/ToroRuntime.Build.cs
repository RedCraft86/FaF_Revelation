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
                "Slate",
                "SlateCore",
                "MovieScene",
                "LevelSequence",
                "GameplayTags",
                "ToroCore"
            }
        );
    }
}