// Copyright (C) RedCraft86. All Rights Reserved.

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
				"Engine"
			]
		);
	}
}
