// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Helpers/GameplayTagHelpers.h"
#include "UDSInterface.generated.h"

DECLARE_GAMEPLAY_TAG_BASE(Weather)
DECLARE_GAMEPLAY_TAG_CHILD(Weather, ClearSkies)
DECLARE_GAMEPLAY_TAG_CHILD(Weather, Cloudy)
DECLARE_GAMEPLAY_TAG_CHILD(Weather, Overcast)
DECLARE_GAMEPLAY_TAG_CHILD(Weather, PartlyCloudy)
DECLARE_GAMEPLAY_TAG_CHILD(Weather, FogThick)
DECLARE_GAMEPLAY_TAG_CHILD(Weather, FogThin)
DECLARE_GAMEPLAY_TAG_CHILD(Weather, Rain)
DECLARE_GAMEPLAY_TAG_CHILD(Weather, RainLight)
DECLARE_GAMEPLAY_TAG_CHILD(Weather, Thunderstorm)

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

UCLASS(Abstract, Blueprintable)
class TOROCORE_API UUDSSetterObject final : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent)
		void SetSettings(const FUDSSettings& InSettings);
};

// UINTERFACE()
// class UUDSInterface : public UInterface
// {
// 	GENERATED_BODY()
// };
//
// class TOROCORE_API IUDSInterface
// {
// 	GENERATED_BODY()
//
// public:
//
// 	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = UDSInterface)
// 	void SetSettings(const FUDSSettings& InSettings);
//
// 	static void SetSettings(UObject* Target, const FUDSSettings& InSettings)
// 	{
// 		if (IsValid(Target) && Target->Implements<UUDSInterface>())
// 		{
// 			IUDSInterface::Execute_SetSettings(Target, InSettings);
// 		}
// 	}
// };