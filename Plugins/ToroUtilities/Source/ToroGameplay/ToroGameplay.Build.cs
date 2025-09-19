// Copyright (C) RedCraft86. All Rights Reserved.

using UnrealBuildTool;

public class ToroGameplay : ModuleRules
{
    public ToroGameplay(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            [
                "Core",
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "Narrative",
                "DeveloperSettings",
                
                "ToroCore",
                "ToroRuntime",
                "ToroFramework"
            ]
        );
    }
}