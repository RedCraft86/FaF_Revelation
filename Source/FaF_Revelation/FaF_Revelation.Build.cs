// Copyright (C) RedCraft86. All Rights Reserved.

using UnrealBuildTool;

public class FaF_Revelation : ModuleRules
{
	public FaF_Revelation(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"Json",
			"UMG",
			"Slate",
			"SlateCore",
			"MovieScene",
			"LevelSequence",
			"JsonUtilities",
			"EngineSettings",
			"AudioExtensions",
			"ApplicationCore",
			"DeveloperSettings",
			"CinematicCamera",
			"MetasoundEngine",
			"EnhancedInput",
			"EngineCameras",
			"GameplayTags",
			"PhysicsCore",
			"InputCore",
			"CommonUI",
			"ToroCore",
			"ToroRuntime",
			"DataGraph",
			"Narrative",
			"ExpressiveText",
			"EnhancedCodeFlow",
		});
	}
}
