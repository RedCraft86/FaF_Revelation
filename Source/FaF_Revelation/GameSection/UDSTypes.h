// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "NativeGameplayTags.h"
#include "UDSTypes.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Weather);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_FogThin);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_FogThick);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Cloudy);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Overcast);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_PartlyCloudy);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Thunderstorm);

UENUM(BlueprintType, DisplayName = "UDS Cloud Type")
enum class EUDSCloudType : uint8
{
	Volumetric,
	Dynamic2D,
	None,
};

USTRUCT(BlueprintType, DisplayName = "UDS Settings")
struct FAF_REVELATION_API FUDSSettings
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
		, WeatherPreset(Tag_FogThick)
	{}
};

UCLASS(Abstract, Blueprintable)
class FAF_REVELATION_API UUDSSetterObject final : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent)
		void SetCustomSettings(const UObject* WorldContext, const FUDSSettings& InSettings);

	virtual UWorld* GetWorld() const override;
};
