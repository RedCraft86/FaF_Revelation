// Copyright (C) RedCraft86, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FaFRevEditor : ModuleRules
{
    public FaFRevEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bUseUnity = true;

        PublicDependencyModuleNames.AddRange(
            new[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "UnrealEd",
                "AssetDefinition",
                "FaFRevGame",
                "ToroEditor"
            }
        );
    }
}