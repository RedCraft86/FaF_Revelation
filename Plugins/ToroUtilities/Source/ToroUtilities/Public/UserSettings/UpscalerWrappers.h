// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

// #include "NISLibrary.h"
// #include "DLSSLibrary.h"
// #include "StreamlineLibraryReflex.h"
// #include "StreamlineLibraryDeepDVC.h"
// #include "StreamlineLibraryDLSSG.h"
// #include "XeSSBlueprintLibrary.h"

namespace XeSS
{
	static bool IsXeSSSupported()
	{
		return false; // UXeSSBlueprintLibrary::IsXeSSSupported();
	}
	
	static void SetXeSSMode(const uint8 InMode)
	{
		// EXeSSQualityMode Mode = EXeSSQualityMode::Off;
		// if (IsXeSSSupported())
		// {
		// 	switch (InMode)
		// 	{
		// 	case 1: Mode = EXeSSQualityMode::UltraPerformance;	break;
		// 	case 2: Mode = EXeSSQualityMode::Performance;		break;
		// 	case 3: Mode = EXeSSQualityMode::Balanced;			break;
		// 	case 4: Mode = EXeSSQualityMode::Quality;			break;
		// 	case 5: Mode = EXeSSQualityMode::UltraQuality;		break;
		// 	case 6: Mode = EXeSSQualityMode::UltraQualityPlus;	break;
		// 	case 7: Mode = EXeSSQualityMode::AntiAliasing;		break;
		// 	default: break;
		// 	}
		// }
		// UXeSSBlueprintLibrary::SetXeSSQualityMode(Mode);
	}
}
