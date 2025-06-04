// Copyright (C) RedCraft86. All Rights Reserved.

using UnrealBuildTool;

public class FaFRevEditor : ModuleRules
{
    public FaFRevEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore"
            }
        );
    }
}