// Copyright (C) RedCraft86. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class FaF_RevelationEditorTarget : TargetRules
{
	public FaF_RevelationEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;
		ExtraModuleNames.AddRange(new [] { "GameRuntime", "GameEditor" });
	}
}
