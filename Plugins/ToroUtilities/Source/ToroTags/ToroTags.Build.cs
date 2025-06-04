// Copyright (C) RedCraft86. All Rights Reserved.

using UnrealBuildTool;

public class ToroTags : ModuleRules
{
    public ToroTags(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "GameplayTags"
            }
        );
    }
}