// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserSettingTypes.h"
#include "Libraries/ToroConsoleLibrary.h"

#include "XeSSBlueprintLibrary.h"
#include "XeFGBlueprintLibrary.h"
#include "XeLLBlueprintLibrary.h"

#include "DLSSLibrary.h"
#include "StreamlineLibraryDLSSG.h"
#include "StreamlineLibraryReflex.h"

namespace ImageFidelity
{
    TORORUNTIME_API inline void SetAntiAliasing(const EImageFidelityMode Method)
    {
        UToroConsoleLibrary::SetCVarInt("r.AntiAliasingMethod", ConvertImageFidelity(Method));
    }

    TORORUNTIME_API inline bool IsUsingAnyExternalUpscaler()
    {
        return UToroConsoleLibrary::GetCVarBool("r.FidelityFX.FSR3.Enabled")
            || UXeSSBlueprintLibrary::GetXeSSQualityMode() != EXeSSQualityMode::Off
            || UDLSSLibrary::IsDLSSEnabled();
    }

    TORORUNTIME_API inline bool IsUsingAnyFrameGen()
    {
        return UToroConsoleLibrary::GetCVarBool("r.FidelityFX.FI.Enabled")
            || UXeFGBlueprintLibrary::GetXeFGMode() != EXeFGMode::Off
            || UStreamlineLibraryDLSSG::GetDLSSGMode() != EStreamlineDLSSGMode::Off;
    }

    namespace FSR
    {
        TORORUNTIME_API inline void Initialize()
        {
            UToroConsoleLibrary::SetCVarBool("r.FidelityFX.FI.EnabledInEditorViewport", true);
            UToroConsoleLibrary::SetCVarBool("r.FidelityFX.FI.UpdateGlobalFrameTime", true);
        }

        TORORUNTIME_API inline void SetEnabled(const bool bEnabled)
        {
            UToroConsoleLibrary::SetCVarBool("r.FidelityFX.FSR3.Enabled", bEnabled);
        }

        TORORUNTIME_API inline void SetQuality(const uint8 Quality)
        {
            UToroConsoleLibrary::SetCVarInt("r.FidelityFX.FSR3.QualityMode",
                FMath::Clamp((int32)Quality, 0, 4));
        }

        TORORUNTIME_API inline void SetSharpness(const uint8 Value)
        {
            UToroConsoleLibrary::SetCVarFloat("r.FidelityFX.FSR3.Sharpness",
                FMath::Clamp(Value, 0, 100) * 0.01f);
        }

        TORORUNTIME_API inline void SetFrameGenEnabled(const bool bEnabled)
        {
            UToroConsoleLibrary::SetCVarBool("r.FidelityFX.FI.Enabled", bEnabled);
        }
    }

    namespace XeSS
    {
        namespace SR
        {
            TORORUNTIME_API inline bool IsSupported()
            {
                return UXeSSBlueprintLibrary::IsXeSSSupported();
            }

            TORORUNTIME_API inline void SetMode(const bool bEnabled, const uint8 Quality)
            {
                if (!bEnabled || !IsSupported())
                {
                    UXeSSBlueprintLibrary::SetXeSSQualityMode(EXeSSQualityMode::Off);
                }
                else
                {
                    const uint8 ClampedQuality = FMath::Clamp((int32)Quality, 0, 6);
                    UXeSSBlueprintLibrary::SetXeSSQualityMode(static_cast<EXeSSQualityMode>(ClampedQuality + 1));
                }
            }
        }

        namespace FG
        {
            TORORUNTIME_API inline bool IsSupported()
            {
                return UXeFGBlueprintLibrary::IsXeFGSupported();
            }

            TORORUNTIME_API inline void SetEnabled(const bool bEnabled)
            {
                UXeFGBlueprintLibrary::SetXeFGMode(bEnabled && IsSupported() ? EXeFGMode::On : EXeFGMode::Off);
            }
        }

        namespace LL
        {
            TORORUNTIME_API inline bool IsSupported()
            {
                return UXeLLBlueprintLibrary::IsXeLLSupported();
            }

            TORORUNTIME_API inline void SetEnabled(const bool bEnabled)
            {
                UXeLLBlueprintLibrary::SetXeLLMode(bEnabled && IsSupported() ? EXeLLMode::On : EXeLLMode::Off);
            }
        }
    }

    namespace DLSS
    {
        namespace SR
        {
            TORORUNTIME_API inline bool IsSupported()
            {
                return UDLSSLibrary::IsDLSSSupported();
            }

            TORORUNTIME_API inline void SetMode(const bool bEnabled, const uint8 Quality, const FVector2D ScreenRes)
            {
                if (!bEnabled || !IsSupported())
                {
                    UDLSSLibrary::EnableDLSS(false);
                    UToroConsoleLibrary::SetCVarFloat("r.ScreenPercentage", 100.0f);
                }
                else
                {
                    UDLSSLibrary::EnableDLSS(true);

                    bool bSupported, bFixedScreenPercent;
                    float OptimalScreenPercent, MinPercent, MaxPercent, Sharpness;
                    const uint8 ClampedQuality = FMath::Clamp((int32)Quality, 0, 6);

                    UDLSSLibrary::GetDLSSModeInformation(static_cast<UDLSSMode>(ClampedQuality + 1),
                        ScreenRes, bSupported, OptimalScreenPercent,
                        bFixedScreenPercent, MinPercent, MaxPercent, Sharpness);

                    UToroConsoleLibrary::SetCVarFloat("r.ScreenPercentage", OptimalScreenPercent);
                }
            }
        }

        namespace FG
        {
            TORORUNTIME_API inline bool IsSupported()
            {
                return UStreamlineLibraryDLSSG::IsDLSSGSupported();
            }

            TORORUNTIME_API inline void SetMode(const uint8 Mode)
            {
                if (Mode > 0 && IsSupported())
                {
                    EStreamlineDLSSGMode FGMode;
                    switch (Mode)
                    {
                        case 1:  FGMode = EStreamlineDLSSGMode::Auto; break;
                        case 2:  FGMode = EStreamlineDLSSGMode::On2X; break;
                        case 3:  FGMode = EStreamlineDLSSGMode::On3X; break;
                        case 4:  FGMode = EStreamlineDLSSGMode::On4X; break;
                        default: FGMode = EStreamlineDLSSGMode::Off;  break;
                    }
                    UStreamlineLibraryDLSSG::SetDLSSGMode(FGMode);
                }
                else
                {
                    UStreamlineLibraryDLSSG::SetDLSSGMode(EStreamlineDLSSGMode::Off);
                }
            }
        }

        namespace RR
        {
            TORORUNTIME_API inline bool IsSupported()
            {
                return UDLSSLibrary::IsDLSSRRSupported();
            }

            TORORUNTIME_API inline void SetMode(const bool bEnabled)
            {
                UDLSSLibrary::EnableDLSSRR(bEnabled && IsSupported());
            }
        }

        namespace Reflex
        {
            TORORUNTIME_API inline bool IsSupported()
            {
                return UStreamlineLibraryReflex::IsReflexSupported();
            }

            TORORUNTIME_API inline void SetMode(const uint8 Mode)
            {
                if (Mode > 0 && IsSupported())
                {
                    EStreamlineReflexMode RFMode;
                    switch (Mode)
                    {
                    case 1:  RFMode = EStreamlineReflexMode::Enabled; break;
                    case 2:  RFMode = EStreamlineReflexMode::Boost; break;
                    default: RFMode = EStreamlineReflexMode::Off;  break;
                    }
                    UStreamlineLibraryReflex::SetReflexMode(RFMode);
                }
                else
                {
                    UStreamlineLibraryReflex::SetReflexMode(EStreamlineReflexMode::Off);
                }
            }
        }
    }
}
