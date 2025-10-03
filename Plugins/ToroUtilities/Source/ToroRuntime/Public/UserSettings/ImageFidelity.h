// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserSettingTypes.h"
#include "XeSSBlueprintLibrary.h"
#include "XeFGBlueprintLibrary.h"
#include "XeLLBlueprintLibrary.h"
#include "Libraries/ToroConsoleLibrary.h"

namespace ImageFidelity
{
    TORORUNTIME_API inline void SetAntiAliasing(const EImageFidelityMode Method)
    {
        UToroConsoleLibrary::SetCVarInt("r.AntiAliasingMethod", ConvertImageFidelity(Method));
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
}
