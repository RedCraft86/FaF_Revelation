// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "NISLibrary.h"
#include "DLSSLibrary.h"
#include "StreamlineLibraryReflex.h"
#include "StreamlineLibraryDeepDVC.h"
#include "StreamlineLibraryDLSSG.h"
#include "XeSSBlueprintLibrary.h"

namespace XeSS
{
	static bool IsXeSSSupported()
	{
		return UXeSSBlueprintLibrary::IsXeSSSupported();
	}
	
	static void SetXeSSMode(const uint8 InMode)
	{
		EXeSSQualityMode Mode = EXeSSQualityMode::Off;
		if (IsXeSSSupported())
		{
			switch (InMode)
			{
			case 1: Mode = EXeSSQualityMode::UltraPerformance;	break;
			case 2: Mode = EXeSSQualityMode::Performance;		break;
			case 3: Mode = EXeSSQualityMode::Balanced;			break;
			case 4: Mode = EXeSSQualityMode::Quality;			break;
			case 5: Mode = EXeSSQualityMode::UltraQuality;		break;
			case 6: Mode = EXeSSQualityMode::UltraQualityPlus;	break;
			case 7: Mode = EXeSSQualityMode::AntiAliasing;		break;
			default: break;
			}
		}
		UXeSSBlueprintLibrary::SetXeSSQualityMode(Mode);
	}
}

namespace Nvidia
{
	namespace NIS
	{
		static bool IsNISSupported()
		{
			return UNISLibrary::IsNISSupported();
		}

		static void SetNISMode(const uint8 InMode)
		{
			UNISMode Mode = UNISMode::Off;
			if (UNISLibrary::IsNISSupported())
			{
				switch (InMode)
				{
				case 1: Mode = UNISMode::UltraQuality;	break;
				case 2: Mode = UNISMode::Quality;		break;
				case 3: Mode = UNISMode::Balanced;		break;
				case 4: Mode = UNISMode::Performance;	break;
				default: break;
				}
			}
			UNISLibrary::SetNISMode(Mode);
		}

		static void SetNISSharpness(const float InValue)
		{
			UNISLibrary::SetNISSharpness(FMath::Clamp(InValue, 0.0f, 1.0f));
		}
	}

	namespace DLSS
	{
		static bool IsDLSSSupported()
		{
			return UDLSSLibrary::IsDLSSSupported();
		}

		static void SetDLSSMode(const uint8 InMode)
		{
			UDLSSMode Mode = UDLSSMode::Off;
			switch (InMode)
			{
			case 1: Mode = UDLSSMode::DLAA;				break;
			case 2: Mode = UDLSSMode::UltraQuality;		break;
			case 3: Mode = UDLSSMode::Quality;			break;
			case 4: Mode = UDLSSMode::Balanced;			break;
			case 5: Mode = UDLSSMode::Performance;		break;
			case 6: Mode = UDLSSMode::UltraPerformance;	break;
			default: break;
			}
			UDLSSLibrary::EnableDLSS(InMode != 0);
			UDLSSLibrary::SetDLSSMode(Mode);
		}

		static bool IsDLSSRRSupported()
		{
			return UDLSSLibrary::IsDLSSRRSupported();
		}

		static void SetDLSSRREnabled(const bool bEnabled)
		{
			UDLSSLibrary::EnableDLSSRR(IsDLSSRRSupported() && bEnabled);
		}

		static bool IsDLSSGSupported()
		{
			return UStreamlineLibraryDLSSG::IsDLSSGSupported();
		}

		static void SetFrameGenMode(const uint8 InMode)
		{
			EStreamlineDLSSGMode Mode = EStreamlineDLSSGMode::Off;
			if (IsDLSSGSupported())
			{ 
				switch (InMode)
				{
				case 1: Mode = EStreamlineDLSSGMode::Auto;	break;
				case 2: Mode = EStreamlineDLSSGMode::On2X;	break;
				case 3: Mode = EStreamlineDLSSGMode::On3X;	break;
				case 4: Mode = EStreamlineDLSSGMode::On4X;	break;
				default: break;
				}
			}
			UStreamlineLibraryDLSSG::SetDLSSGMode(Mode);
		}
	}

	namespace Streamline
	{
		static bool IsReflexSupported()
		{
			return UStreamlineLibraryReflex::IsReflexSupported();
		}

		static void SetReflexMode(const uint8 InMode)
		{
			EStreamlineReflexMode Mode = EStreamlineReflexMode::Off;
			if (IsReflexSupported())
			{
				switch (InMode)
				{
				case 1: Mode = EStreamlineReflexMode::Enabled;	break;
				case 2: Mode = EStreamlineReflexMode::Boost;	break;
				default: break;
				}
			}
			UStreamlineLibraryReflex::SetReflexMode(Mode);
		}

		static bool IsDeepDVCSupported()
		{
			return UStreamlineLibraryDeepDVC::IsDeepDVCSupported();
		}

		static void SetDynamicVibrance(const bool bEnabled, const float Intensity, const float Saturation)
		{
			const EStreamlineDeepDVCMode Mode = bEnabled && IsDeepDVCSupported()
				? EStreamlineDeepDVCMode::On : EStreamlineDeepDVCMode::Off;

			UStreamlineLibraryDeepDVC::SetDeepDVCMode(Mode);
			UStreamlineLibraryDeepDVC::SetDeepDVCSaturationBoost(Saturation);
			UStreamlineLibraryDeepDVC::SetDeepDVCIntensity(Intensity);
		}
	}
}
