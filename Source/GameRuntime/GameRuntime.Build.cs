// Copyright (C) RedCraft86. All Rights Reserved.

using UnrealBuildTool;

public class GameRuntime : ModuleRules
{
    public GameRuntime(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore"
            }
        );
    }
}