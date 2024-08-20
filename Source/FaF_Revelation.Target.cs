// Copyright (C) RedCraft86. All Rights Reserved.

using UnrealBuildTool;

public class FaF_RevelationTarget : TargetRules
{
	public FaF_RevelationTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		ExtraModuleNames.Add("FaF_Rev");
	}
}
