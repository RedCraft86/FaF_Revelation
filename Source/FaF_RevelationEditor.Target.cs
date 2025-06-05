// Copyright (C) RedCraft86. All Rights Reserved.

using UnrealBuildTool;

public class FaF_RevelationEditorTarget : TargetRules
{
	public FaF_RevelationEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_6;
		ExtraModuleNames.AddRange(new[] { "FaFRevEditor", "FaFRevGame" });
	}
}
