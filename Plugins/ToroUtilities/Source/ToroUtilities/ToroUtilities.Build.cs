// Copyright (C) RedCraft86. All Rights Reserved.

using UnrealBuildTool;

public class ToroUtilities : ModuleRules
{
    public ToroUtilities(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bUseUnity = false;

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
                "GameplayTags",
                "MetasoundEngine",
                "AudioExtensions",
                "DeveloperSettings",
                "EnhancedCodeFlow",
                "ExpressiveText",
                "ToroCore", 
            }
        );
    }
}