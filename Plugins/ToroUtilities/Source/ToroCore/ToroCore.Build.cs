// Copyright (C) RedCraft86. All Rights Reserved.

using UnrealBuildTool;

public class ToroCore : ModuleRules
{
    public ToroCore(ReadOnlyTargetRules Target) : base(Target)
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
                "InputCore",
                "NavigationSystem",
                "GameplayTags"
            }
        );
    }
}