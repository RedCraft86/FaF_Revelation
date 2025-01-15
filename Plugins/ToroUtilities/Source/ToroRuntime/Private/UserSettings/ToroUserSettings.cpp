// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserSettings/ToroUserSettings.h"
#include "ToroRuntimeSettings.h"
#include "AudioDevice.h"

DEFINE_PROPERTY_SETTER(bool, ShowFPS, {

})

DEFINE_PROPERTY_SETTER(FString, Username, {
	
})

void UToroUserSettings::SetAudioVolume(const ESoundClassType InType, const uint8 InVolume)
{
}

DEFINE_PROPERTY_SETTER(uint8, FieldOfView, {

})

DEFINE_PROPERTY_SETTER(bool, SmoothCamera, {
	
})

DEFINE_PROPERTY_SETTER(float, SensitivityX, {
	
})

DEFINE_PROPERTY_SETTER(float, SensitivityY, {
	
})

DEFINE_PROPERTY_SETTER(float, Gamma, {
	
})

DEFINE_PROPERTY_SETTER(uint8, Brightness, {
	
})

DEFINE_PROPERTY_SETTER(bool, FancyBloom, {
	
})

DEFINE_PROPERTY_SETTER(bool, ScreenSpaceFogScattering, {
	
})

DEFINE_PROPERTY_SETTER(uint8, MotionBlurAmount, {
	
})

DEFINE_PROPERTY_SETTER(bool, LumenGI, {
	
})
DEFINE_SETTER(uint8, MotionBlur, 
	if (InValue == 0)
	{
		SET_CONSOLE_VAR(TEXT("r.DefaultFeature.MotionBlur"), 0);
	}
	else
	{
		OnDynamicSettingsChanged.Broadcast(this);
	}
)

DEFINE_SETTER_DYNAMIC(uint8, LumenGI)
DEFINE_SETTER_DYNAMIC(uint8, LumenReflection)
DEFINE_SETTER(bool, HitLightingReflections)

DEFINE_PROPERTY_SETTER(uint8, LumenGIQuality, {
	
})

DEFINE_PROPERTY_SETTER(bool, LumenReflections, {
	
})

DEFINE_PROPERTY_SETTER(bool, HitLightingReflections, {
	
})

DEFINE_PROPERTY_SETTER(uint8, LumenReflectionQuality, {
	
})

DEFINE_PROPERTY_SETTER(EColorBlindMode, ColorBlindMode, {
	
})

DEFINE_PROPERTY_SETTER(uint8, ColorBlindIntensity, {
	
})

DEFINE_PROPERTY_SETTER(bool, RTXDynamicVibrance, {
	
})

DEFINE_PROPERTY_SETTER(float, DynamicVibranceIntensity, {
	
})

DEFINE_PROPERTY_SETTER(float, DynamicVibranceSaturation, {
	
})

DEFINE_PROPERTY_SETTER(EImageFidelityMode, ImageFidelityMode, {
	
})

DEFINE_PROPERTY_SETTER(uint8, FXAADithering, {
	
})

DEFINE_PROPERTY_SETTER(uint8, TAAUpsampling, {
	
})

DEFINE_PROPERTY_SETTER(uint8, TSRResolution, {
	
})

DEFINE_PROPERTY_SETTER(uint8, SMAAQuality, {
	
})

DEFINE_PROPERTY_SETTER(uint8, SMAAEdgeMode, {
	
})

DEFINE_PROPERTY_SETTER(uint8, DLSSQuality, {
	
})

DEFINE_PROPERTY_SETTER(float, DLSSSharpness, {
	
})

DEFINE_PROPERTY_SETTER(bool, DLSSRayReconstruction, {
	
})

DEFINE_PROPERTY_SETTER(bool, DLSSFrameGeneration, {
	
})

DEFINE_PROPERTY_SETTER(uint8, NvidiaReflex, {
	
})

DEFINE_PROPERTY_SETTER(uint8, FSRQuality, {
	
})

DEFINE_PROPERTY_SETTER(float, FSRSharpness, {
	
})

DEFINE_PROPERTY_SETTER(bool, FSRFrameGeneration, {
	
})

DEFINE_PROPERTY_SETTER(uint8, XeSSQuality, {
	
})

DEFINE_PROPERTY_SETTER(uint8, NISQuality, {
	
})

DEFINE_PROPERTY_SETTER(float, NISSharpness, {
	
})

DEFINE_PROPERTY_SETTER(float, NISScreenPercentage, {
	
})

void UToroUserSettings::ApplyAudioSettings() const
{
#if WITH_EDITOR
	if (!FApp::IsGame()) return;
#endif
	const UWorld* World = GetWorld();
	FAudioDeviceHandle AudioDevice = World ? World->GetAudioDevice() : FAudioDeviceHandle();
	if (!AudioDevice.IsValid()) return;
	
	const UToroRuntimeSettings* Settings = UToroRuntimeSettings::Get();
	if (USoundMix* SoundMix = Settings ? Settings->BaseSoundMix.LoadSynchronous() : nullptr)
	{
		for (const ESoundClassType Type : TEnumRange<ESoundClassType>())
		{
			if (USoundClass* SoundClass = Settings->SoundClasses[static_cast<uint8>(Type)].LoadSynchronous())
			{
				AudioDevice->SetSoundMixClassOverride(SoundMix, SoundClass,
					GetAudioVolume(Type), 1.0f, 0.5f, true);
			}
		}
	}
}

UWorld* UToroUserSettings::GetWorld() const
{
	UWorld* World = Super::GetWorld();
	if (!World) World = GEngine ? GEngine->GetCurrentPlayWorld() : GWorld;
	return World;
}
