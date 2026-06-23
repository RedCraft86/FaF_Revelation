// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

using UnrealBuildTool;

public class FaF_RevelationEditorTarget : TargetRules
{
	public FaF_RevelationEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		ExtraModuleNames.AddRange(
			[
				"FaF_Revelation",
				"GameRuntime",
				"GameEditor"
			]
		);
	}
}
