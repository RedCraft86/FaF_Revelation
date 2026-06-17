// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class FaF_RevelationEditorTarget : TargetRules
{
	public FaF_RevelationEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V7;
		ExtraModuleNames.AddRange(
			[
				"FaF_Revelation"
			]
		);
	}
}
