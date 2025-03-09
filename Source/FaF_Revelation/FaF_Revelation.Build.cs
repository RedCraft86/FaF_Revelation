// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FaF_Revelation : ModuleRules
{
	public FaF_Revelation(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core", 
			"Slate", 
			"SlateCore", 
			"CoreUObject", 
			"EnhancedInput",
			"InputCore", 
			"Engine",
			"ToroCore",
			"ToroRuntime",
		});
	}
}
