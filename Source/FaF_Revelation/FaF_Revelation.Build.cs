// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

using UnrealBuildTool;

public class FaF_Revelation : ModuleRules
{
	public FaF_Revelation(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(
			[
				"Core", 
				"CoreUObject", 
				"Engine", 
				"InputCore",
				"GameplayTags",
				"ToroRuntime"
			]
		);
	}
}
