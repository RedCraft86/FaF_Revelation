// Copyright (C) RedCraft86. All Rights Reserved.

using UnrealBuildTool;

public class ToroGameplay : ModuleRules
{
    public ToroGameplay(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateDependencyModuleNames.AddRange(
            new[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "GameplayTags",
                "ToroCore"
            }
        );
    }
}