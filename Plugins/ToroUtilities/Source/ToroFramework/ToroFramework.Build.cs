// Copyright (C) RedCraft86. All Rights Reserved.

using UnrealBuildTool;

public class ToroFramework : ModuleRules
{
    public ToroFramework(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            [
                "Core",
                "CoreUObject",
                "Engine",
                "UMG",
                "Slate",
                "SlateCore",
                "EnhancedInput",
                "InputCore",
                "CommonUI",
                "MovieScene",
                "LevelSequence",
                "GameplayTags",
                "EngineSettings",
                "DeveloperSettings",
                
                "ToroCore"
            ]
        );
    }
}