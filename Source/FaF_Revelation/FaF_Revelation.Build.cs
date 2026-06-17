// Fill out your copyright notice in the Description page of Project Settings.

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
				"InputCore"
			]
		);
	}
}
