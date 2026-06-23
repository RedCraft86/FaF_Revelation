// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

using UnrealBuildTool;

public class FaF_RevelationTarget : TargetRules
{
	public FaF_RevelationTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		ExtraModuleNames.AddRange(
			[
				"FaF_Revelation",
				"GameRuntime"
			]
		);
	}
}
