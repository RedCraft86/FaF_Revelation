// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class FaF_RevelationTarget : TargetRules
{
	public FaF_RevelationTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V7;
		ExtraModuleNames.AddRange(
			[
				"FaF_Revelation"
			]
		);
	}
}
