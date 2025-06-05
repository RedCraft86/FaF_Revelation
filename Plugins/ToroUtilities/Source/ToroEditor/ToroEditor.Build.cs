// Copyright (C) RedCraft86. All Rights Reserved.

using UnrealBuildTool;

public class ToroEditor : ModuleRules
{
    public ToroEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new[]
            {
                "Core",
                "CoreUObject",
                "UnrealEd",
                "Engine",
                "Slate",
                "SlateCore",
                "Projects",
                "DeveloperSettings",
                "EditorScriptingUtilities",
                "EditorSubsystem",
                "ToroCore"
            }
        );
    }
}