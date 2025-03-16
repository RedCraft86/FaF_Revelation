// Copyright (C) RedCraft86. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class ToroCore : ModuleRules
{
    public ToroCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bUseUnity = false;
        
        // Include all the immediate folders under .../ToroCore/Public/
        PublicIncludePaths.AddRange(
            Directory.GetDirectories(Path.Combine(PluginDirectory, "Source/ToroCore/Public/"))
        );
        
        PublicDependencyModuleNames.AddRange(
            new[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "UMG",
                "Json",
                "Slate",
                "SlateCore",
                "InputCore",
                "JsonUtilities",
                "NavigationSystem",
                "ProceduralMeshComponent",
            }
        );
        
        if (Target.Type == TargetType.Editor)
        {
            PublicDependencyModuleNames.AddRange(
                new[]
                {
                    "UnrealEd",
                    "EditorScriptingUtilities"
                }
            );
        }
    }
}