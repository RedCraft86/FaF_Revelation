// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

using UnrealBuildTool;

public class GameRuntime : ModuleRules
{
    public GameRuntime(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.Add(ModuleDirectory);

        PublicDependencyModuleNames.AddRange(
            [
                "Core",
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore", 
                "InputCore",
                "GameplayTags",
                "DeveloperSettings",
                "AsyncMessageSystem",
                "EngineSettings",
                "ToroRuntime",
                "ToroCore",
                "UE5Coro",
                "Flow"
            ]
        );
    }
}