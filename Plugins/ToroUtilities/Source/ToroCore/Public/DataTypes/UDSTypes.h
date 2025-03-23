// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroCore.h"
#include "GameplayTagContainer.h"
#include "UDSTypes.generated.h"

UE_DECLARE_GAMEPLAY_TAG_BASE(Weather)
UE_DECLARE_GAMEPLAY_TAG_CHILD(ClearSkies,	Weather)
UE_DECLARE_GAMEPLAY_TAG_CHILD(Cloudy,		Weather)
UE_DECLARE_GAMEPLAY_TAG_CHILD(Overcast,		Weather)
UE_DECLARE_GAMEPLAY_TAG_CHILD(PartlyCloudy, Weather)
UE_DECLARE_GAMEPLAY_TAG_CHILD(FogThick,		Weather)
UE_DECLARE_GAMEPLAY_TAG_CHILD(FogThin,		Weather)
UE_DECLARE_GAMEPLAY_TAG_CHILD(Rain,			Weather)
UE_DECLARE_GAMEPLAY_TAG_CHILD(RainLight,	Weather)
UE_DECLARE_GAMEPLAY_TAG_CHILD(Thunderstorm, Weather)

UENUM(BlueprintType, DisplayName = "UDS Cloud Type")
enum class EUDSCloudType : uint8
{
	Volumetric,
	Dynamic2D,
	None,
};

USTRUCT(BlueprintType, DisplayName = "UDS Settings")
struct TOROCORE_API FUDSSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float NightBrightness;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float FogBrightness;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float SkyLightIntensity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		EUDSCloudType CloudType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (ClampMin = 0.0f, UIMin = 0.0f, ClampMax = 2400.0f, UIMax = 2400.0f))
		float TimeOfDay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (ClampMin = 0.1f, UIMin = 0.1f, ClampMax = 2.0f, UIMax = 2.0f))
		float WeatherVolume;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (ClampMin = 0.5f, UIMin = 0.5f, ClampMax = 10.0f, UIMax = 10.0f))
		float WeatherFadeTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (Categories = "Weather"))
		FGameplayTag WeatherPreset;

	FUDSSettings()
		: NightBrightness(2.5f)
		, FogBrightness(1.0f)
		, SkyLightIntensity(4.0f)
		, CloudType(EUDSCloudType::None)
		, TimeOfDay(0.0f)
		, WeatherVolume(1.0f)
		, WeatherFadeTime(1.0f)
		, WeatherPreset(Tag_Weather_FogThick)
	{}
};