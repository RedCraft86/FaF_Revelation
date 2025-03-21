// Copyright (C) RedCraft86. All Rights Reserved.

using UnrealBuildTool;

public class GameEditor : ModuleRules
{
    public GameEditor(ReadOnlyTargetRules Target) : base(Target)
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
                "ToroEditor",
                "GameRuntime"
            }
        );
    }
}