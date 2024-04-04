// Copyright (C) RedCraft86. All Rights Reserved.

using UnrealBuildTool;

public class FaF_Rev : ModuleRules
{
	public FaF_Rev(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicDependencyModuleNames.AddRange(
			new[]
			{
				"Core", 
				"CoreUObject", 
				"Engine", 
				"InputCore", 
				"EnhancedInput",
				"GTRuntime",
				"GTCore"
			}
		);
	}
}
