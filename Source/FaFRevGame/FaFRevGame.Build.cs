// Copyright (C) RedCraft86. All Rights Reserved.

using UnrealBuildTool;

public class FaFRevGame : ModuleRules
{
    public FaFRevGame(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bUseUnity = false;

        PublicDependencyModuleNames.AddRange(
            new[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "EngineCameras",
                "GameplayTags",
                "ToroCore",
                "ToroUtilities"
            }
        );
    }
}