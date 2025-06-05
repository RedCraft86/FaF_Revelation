// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Helpers/GameplayTagMacros.h"
#include "UDSSetter.generated.h"

namespace WeatherTags
{
	TOROCORE_API DECLARE_GAMEPLAY_TAG(Weather)
	TOROCORE_API DECLARE_GAMEPLAY_TAG(ClearSkies)
	TOROCORE_API DECLARE_GAMEPLAY_TAG(Cloudy)
	TOROCORE_API DECLARE_GAMEPLAY_TAG(Overcast)
	TOROCORE_API DECLARE_GAMEPLAY_TAG(PartlyCloudy)
	TOROCORE_API DECLARE_GAMEPLAY_TAG(FogThick)
	TOROCORE_API DECLARE_GAMEPLAY_TAG(FogThin)
	TOROCORE_API DECLARE_GAMEPLAY_TAG(Rain)
	TOROCORE_API DECLARE_GAMEPLAY_TAG(RainLight)
	TOROCORE_API DECLARE_GAMEPLAY_TAG(Thunderstorm)

	TOROCORE_API CREATE_TAG_VERIFIER(Weather)
}

UENUM(BlueprintType, DisplayName = "UDS Cloud Type")
enum class EUDSCloudType : uint8
{
	Volumetric,
	Dynamic2D,
	None,
};

USTRUCT(BlueprintType, DisplayName = "UDS Settings")
struct FUDSSettings
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
		, WeatherPreset(WeatherTags::TAG_FogThick)
	{}
};

UCLASS(Abstract, Blueprintable, BlueprintType)
class UUDSSetterObject final : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetSettings(const UObject* ContextObject, const FUDSSettings& InSettings);
};
