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
                "Slate",
                "SlateCore",
                "InputCore", 
                "EnhancedInput",
                "DeveloperSettings",
                
                "ToroCore",
                "ToroRuntime",
                "ToroFramework",
                "ToroGameplay"
            ]
        );
    }
}