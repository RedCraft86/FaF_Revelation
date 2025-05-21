// Copyright (C) RedCraft86, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class ToroCore : ModuleRules
{
    public ToroCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bUseUnity = false;

        PublicIncludePaths.Add(Path.Combine(PluginDirectory, "Source/ToroCore/"));

        PublicDependencyModuleNames.AddRange(
            new[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "InputCore",
                "UMG",
                "CommonUI",
                "GameplayTags",
                "ExpressiveText"
            }
        );
    }
}