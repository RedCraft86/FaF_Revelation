// Copyright (C) RedCraft86. All Rights Reserved.

using UnrealBuildTool;

public class FaFRevGame : ModuleRules
{
    public FaFRevGame(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bUseUnity = false;
        
        PublicIncludePaths.Add(ModuleDirectory);

        PublicDependencyModuleNames.AddRange(
            new[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "PhysicsCore",
                "EnhancedInput",
                "AudioExtensions",
                "MetasoundEngine",
                "DeveloperSettings",
                "EngineCameras",
                "GameplayTags",
                "ToroCore",
                "ToroUtilities"
            }
        );
    }
}