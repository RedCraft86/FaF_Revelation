// Copyright (C) RedCraft86. All Rights Reserved.

using UnrealBuildTool;

public class FaFRevGame : ModuleRules
{
    public FaFRevGame(ReadOnlyTargetRules Target) : base(Target)
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
                "CommonUI",
                "InputCore", 
                "MovieScene",
                "LevelSequence",
                "EnhancedInput",
                "DeveloperSettings",
                "GameplayTags",
                
                "ToroCore",
                "ToroRuntime",
                "EnhancedCodeFlow"
            ]
        );
    }
}