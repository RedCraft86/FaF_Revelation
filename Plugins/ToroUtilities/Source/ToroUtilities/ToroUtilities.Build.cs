// Copyright (C) RedCraft86, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ToroUtilities : ModuleRules
{
    public ToroUtilities(ReadOnlyTargetRules Target) : base(Target)
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
                "InputCore",
                "UMG",
                "Json",
                "JsonUtilities",
                "EnhancedInput",
                "ApplicationCore",
                "ProceduralMeshComponent",
                "DeveloperSettings",
                "EngineSettings",
                "LevelSequence",
                "GameplayTags",
                "MovieScene",
                "AIModule",
                "ToroCore",
                "EnhancedCodeFlow", 
                "NISBlueprint", 
                "DLSSBlueprint", 
                "StreamlineReflexBlueprint", 
                "StreamlineDeepDVCBlueprint", 
                "StreamlineDLSSGBlueprint", 
                "XeSSBlueprint"
            }
        );
        
        if (Target.Type == TargetType.Editor)
        {
            PublicDependencyModuleNames.AddRange(
                new[]
            {
                "UnrealEd",
                "EditorScriptingUtilities"
            });
        }
    }
}