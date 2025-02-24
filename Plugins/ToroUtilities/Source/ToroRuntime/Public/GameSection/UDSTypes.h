// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "NativeGameplayTags.h"
#include "UDSTypes.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Weather);

UENUM(BlueprintType, DisplayName = "UDS Cloud Type")
enum class EUDSCloudType : uint8
{
	Volumetric,
	Dynamic2D,
	None,
};

USTRUCT(BlueprintType, DisplayName = "UDS Settings")
struct TORORUNTIME_API FUDSSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		float NightBrightness;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		float FogBrightness;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		float SkyLightIntensity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		EUDSCloudType CloudType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		float WeatherVolume;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		float WeatherFadeTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (Categories = "Weather"))
		FGameplayTag WeatherPreset;

	FUDSSettings()
		: NightBrightness(2.5f)
		, FogBrightness(1.0f)
		, SkyLightIntensity(4.0f)
		, CloudType(EUDSCloudType::Volumetric)
		, WeatherVolume(1.0f)
		, WeatherFadeTime(1.0f)
	{}
};

UCLASS(Abstract, Blueprintable)
class TORORUNTIME_API UUDSSetterObject final : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent)
		void SetCustomSettings(const UObject* WorldContext, const FUDSSettings& InSettings);

	virtual UWorld* GetWorld() const override;
};
