// Copyright (C) RedCraft86. All Rights Reserved.

using UnrealBuildTool;

public class DataGraph : ModuleRules
{
    public DataGraph(ReadOnlyTargetRules Target) : base(Target)
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