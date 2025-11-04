// Copyright (C) RedCraft86. All Rights Reserved.

using UnrealBuildTool;

public class FaFRevEditor : ModuleRules
{
    public FaFRevEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            [
                "Core",
                "CoreUObject", 
                "UnrealEd",
                "Engine", 
                "Slate",
                "SlateCore",
                "InputCore",
                "FaFRevGame",
                "ToroEditor"
            ]
        );
    }
}