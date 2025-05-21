// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "LightingData.generated.h"

class ULightComponent;
class UPointLightComponent;
class USpotLightComponent;
class URectLightComponent;

USTRUCT(BlueprintType)
struct TOROCORE_API FLightDrawDistance
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightData)
		bool bUseDrawDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightData, meta = (EditCondition = "bUseDrawDistance", ClampMin = 0.0f, UIMin = 0.0f))
		float MaxDrawDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightData, meta = (EditCondition = "bUseDrawDistance", ClampMin = 0.0f, UIMin = 0.0f))
		float MaxDistanceFadeRange;

    FLightDrawDistance()
        : bUseDrawDistance(true)
        , MaxDrawDistance(5000.0f)
        , MaxDistanceFadeRange(1000.0f)
    {}
    
	friend FArchive& operator<<(FArchive& Ar, FLightDrawDistance& DrawDistance)
	{
		Ar << DrawDistance.bUseDrawDistance;
		Ar << DrawDistance.MaxDrawDistance;
		Ar << DrawDistance.MaxDistanceFadeRange;
		return Ar;
	}

	FORCEINLINE float GetMaxDrawDistance() const
	{
		return bUseDrawDistance ? FMath::Max(0.0f, MaxDrawDistance) : 0.0f;
	}

	FORCEINLINE float GetMaxDistanceFadeRange() const
	{
		return bUseDrawDistance ? FMath::Max(0.0f, MaxDistanceFadeRange) : 0.0f;
	}

    FORCEINLINE bool IsValidData() const
    {
	    return MaxDistanceFadeRange < MaxDrawDistance && MaxDistanceFadeRange + MaxDrawDistance > 0.0f;
	}
};

USTRUCT(BlueprintType)
struct TOROCORE_API FBaseLightData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightData, meta = (DisplayPriority = 0))
        ELightUnits IntensityUnits;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightData, meta = (ClampMin = 0.0f, UIMin = 0.0f, ShouldShowInViewport = true, DisplayPriority = 0))
        float Intensity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightData, meta = (HideAlphaChannel, ShouldShowInViewport = true, DisplayPriority = 0))
        FLinearColor Color;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightData, meta = (ClampMin = 0.0f, UIMin = 8.0f, UIMax = 16384.0f, ShouldShowInViewport = true, DisplayPriority = 0))
        float AttenuationRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightData, meta = (InlineEditConditionToggle, DisplayPriority = 2))
        bool bUseTemperature;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightData, meta = (UIMin = 1700.0f, UIMax = 12000.0f, ShouldShowInViewport = true, EditCondition = "bUseTemperature", DisplayPriority = 2))
        float Temperature;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightData, meta = (UIMin = 0.0f, UIMax = 6.0f, DisplayPriority = 2))
        float IndirectIntensity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightData, meta = (UIMin = 0.25f, UIMax = 4.0f, DisplayPriority = 2))
        float VolumetricScatteringIntensity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightData, meta = (DisplayPriority = 2))
        bool bLightCastShadows;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightData, meta = (DisplayPriority = 2))
        bool bCastVolumetricShadows;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightData, meta = (ClampMin = 0.0f, ClampMax = 1.0f, UIMin = 0.0f, UIMax = 1.0f, DisplayPriority = 3))
        float SpecularScale;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightData, meta = (DisplayPriority = 3))
        FLightDrawDistance DrawDistance;

    FBaseLightData()
        : IntensityUnits(ELightUnits::Candelas)
        , Intensity(8.0f)
        , Color(FLinearColor::White)
        , AttenuationRadius(1000.0f)
        , bUseTemperature(false)
        , Temperature(6500.0f)
        , IndirectIntensity(1.0f)
        , VolumetricScatteringIntensity(1.0f)
        , bLightCastShadows(true)
        , bCastVolumetricShadows(false)
        , SpecularScale(1.0f)
        , DrawDistance({})
    {}
    
    friend FArchive& operator<<(FArchive& Ar, FBaseLightData& Data)
    {
        Ar << Data.IntensityUnits;
        Ar << Data.Intensity;
        Ar << Data.Color;
        Ar << Data.AttenuationRadius;
        Ar << Data.bUseTemperature;
        Ar << Data.Temperature;
        Ar << Data.IndirectIntensity;
        Ar << Data.VolumetricScatteringIntensity;
        Ar << Data.bLightCastShadows;
        Ar << Data.bCastVolumetricShadows;
        Ar << Data.SpecularScale;
        Ar << Data.DrawDistance;
        return Ar;
    }

    FBaseLightData GetBaseLightData() const;
    void CopyFrom(const FBaseLightData& Data);
    void CopyBaseData(const FBaseLightData& Data);
};

USTRUCT(BlueprintType, DisplayName = "Point Light Data")
struct TOROCORE_API FPointLightData : public FBaseLightData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightData, meta = (ShouldShowInViewport = true, DisplayPriority = 1, ClampMin = 0.0f, UIMin = 0.0f))
        float SourceRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightData, meta = (DisplayPriority = 1, ClampMin = 0.0f, UIMin = 0.0f))
        float SoftSourceRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightData, meta = (DisplayPriority = 1, ClampMin = 0.0f, UIMin = 0.0f))
        float SourceLength;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightData, meta = (InlineEditConditionToggle, DisplayPriority = 3))
        bool bUseInverseSquaredFalloff;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightData, meta = (UIMin = 2.0f, UIMax = 16.0f, EditCondition = "!bUseInverseSquaredFalloff", DisplayPriority = 3))
        float LightFalloffExponent;

    FPointLightData()
        : SourceRadius(0.0f)
        , SoftSourceRadius(0.0f)
        , SourceLength(0.0f)
        , bUseInverseSquaredFalloff(false)
        , LightFalloffExponent(8.0f)
    {
        bUseInverseSquaredFalloff = false;
    }
    
    friend FArchive& operator<<(FArchive& Ar, FPointLightData& Data)
    {
        Ar << Data.SourceRadius;
        Ar << Data.SoftSourceRadius;
        Ar << Data.SourceLength;
        Ar << Data.bUseInverseSquaredFalloff;
        Ar << Data.LightFalloffExponent;
        return Ar;
    }
};

USTRUCT(BlueprintType, DisplayName = "Spot Light Data")
struct TOROCORE_API FSpotLightData : public FBaseLightData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightData, meta = (UIMin = 1.0f, UIMax = 80.0f, ShouldShowInViewport = true, DisplayPriority = 1))
        float InnerConeAngle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightData, meta = (UIMin = 1.0f, UIMax = 80.0f, ShouldShowInViewport = true, DisplayPriority = 1))
        float OuterConeAngle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightData, meta = (ShouldShowInViewport = true, DisplayPriority = 1, ClampMin = 0.0f, UIMin = 0.0f))
        float SourceRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightData, meta = (DisplayPriority = 1, ClampMin = 0.0f, UIMin = 0.0f))
        float SoftSourceRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightData, meta = (DisplayPriority = 1, ClampMin = 0.0f, UIMin = 0.0f))
        float SourceLength;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightData, meta = (InlineEditConditionToggle, DisplayPriority = 3))
        bool bUseInverseSquaredFalloff;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightData, meta = (UIMin = 2.0f, UIMax = 16.0f, EditCondition = "!bUseInverseSquaredFalloff", DisplayPriority = 3))
        float LightFalloffExponent;

    FSpotLightData()
        : InnerConeAngle(0.0f)
        , OuterConeAngle(44.0f)
        , SourceRadius(0.0f)
        , SoftSourceRadius(0.0f)
        , SourceLength(0.0f)
        , bUseInverseSquaredFalloff(false)
        , LightFalloffExponent(8.0f)
    {
        bUseInverseSquaredFalloff = false;
    }
    
    friend FArchive& operator<<(FArchive& Ar, FSpotLightData& Data)
    {
        Ar << Data.InnerConeAngle;
        Ar << Data.OuterConeAngle;
        Ar << Data.SourceLength;
        Ar << Data.bUseInverseSquaredFalloff;
        Ar << Data.LightFalloffExponent;
        return Ar;
    }
};

USTRUCT(BlueprintType, DisplayName = "Rect Light Data")
struct TOROCORE_API FRectLightData : public FBaseLightData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightData, meta = (DisplayPriority = 1, ClampMin = 10.0f, UIMin = 10.0f))
        float SourceWidth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightData, meta = (DisplayPriority = 1, ClampMin = 10.0f, UIMin = 10.0f))
        float SourceHeight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightData, meta = (ClampMin = 0.0f, ClampMax = 90.0f, UIMin = 0.0f, UIMax = 90.0f, DisplayPriority = 1))
        float BarnDoorAngle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightData, meta = (ClampMin = 1.0f, UIMin = 1.0f, DisplayPriority = 1))
        float BarnDoorLength;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LightData, meta = (DisplayPriority = 1))
        TObjectPtr<UTexture> SourceTexture;

    FRectLightData()
        : SourceWidth(64.0f)
        , SourceHeight(64.0f)
        , BarnDoorAngle(88.0f)
        , BarnDoorLength(20.0f)
        , SourceTexture(nullptr)
    {}
    
    friend FArchive& operator<<(FArchive& Ar, FRectLightData& Data)
    {
        Ar << Data.SourceWidth;
        Ar << Data.SourceHeight;
        Ar << Data.BarnDoorAngle;
        Ar << Data.BarnDoorLength;
        Ar << Data.SourceTexture;
        return Ar;
    }
};

UCLASS()
class TOROCORE_API ULightingDataLibrary final : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintPure, Category = "Rendering|Components|Light|Data", DisplayName = "Base Light Data (From Point)", meta = (BlueprintAutocast = true, CompactNodeTitle = "->"))
        static FBaseLightData GetBaseData_Point(const FPointLightData& Target);

    UFUNCTION(BlueprintPure, Category = "Rendering|Components|Light|Data", DisplayName = "Base Light Data (From Spot)", meta = (BlueprintAutocast = true, CompactNodeTitle = "->"))
        static FBaseLightData GetBaseData_Spot(const FSpotLightData& Target);

    UFUNCTION(BlueprintPure, Category = "Rendering|Components|Light|Data", DisplayName = "Base Light Data (From Rect)", meta = (BlueprintAutocast = true, CompactNodeTitle = "->"))
        static FBaseLightData GetBaseData_Rect(const FRectLightData& Target);

    UFUNCTION(BlueprintPure, Category = "Rendering|Components|Light|Data", DisplayName = "Point Light Data (From Base)", meta = (BlueprintAutocast = true, CompactNodeTitle = "->"))
        static FPointLightData GetPointData(const FBaseLightData& Target, const bool bOnlyBasicData);

    UFUNCTION(BlueprintPure, Category = "Rendering|Components|Light|Data", DisplayName = "Spot Light Data (From Base)", meta = (BlueprintAutocast = true, CompactNodeTitle = "->"))
        static FSpotLightData GetSpotData(const FBaseLightData& Target, const bool bOnlyBasicData);

    UFUNCTION(BlueprintPure, Category = "Rendering|Components|Light|Data", DisplayName = "Rect Light Data (From Base)", meta = (BlueprintAutocast = true, CompactNodeTitle = "->"))
        static FRectLightData GetRectData(const FBaseLightData& Target, const bool bOnlyBasicData);
	
	UFUNCTION(BlueprintCallable, Category = "Rendering|Components|Light", meta = (DefaultToSelf = "Target"))
		static void SetLightDrawDistanceSettings(ULightComponent* Target, const FLightDrawDistance& Settings);

	UFUNCTION(BlueprintPure, Category = "Rendering|Components|Light", meta = (DefaultToSelf = "Target"))
		static FLightDrawDistance GetLightDrawDistanceSettings(const ULightComponent* Target);

	UFUNCTION(BlueprintCallable, Category = "Rendering|Components|Light", meta = (DefaultToSelf = "Target"))
		static void SetBaseLightData(ULightComponent* Target, const FBaseLightData& Data);

	UFUNCTION(BlueprintPure, Category = "Rendering|Components|Light", meta = (DefaultToSelf = "Target"))
		static FBaseLightData GetBaseLightData(const ULightComponent* Target);

	UFUNCTION(BlueprintCallable, Category = "Rendering|Components|Light", meta = (DefaultToSelf = "Target"))
		static void SetPointLightData(UPointLightComponent* Target, const FPointLightData& Data);

	UFUNCTION(BlueprintPure, Category = "Rendering|Components|Light", meta = (DefaultToSelf = "Target"))
		static FPointLightData GetPointLightData(const UPointLightComponent* Target);

	UFUNCTION(BlueprintCallable, Category = "Rendering|Components|Light", meta = (DefaultToSelf = "Target"))
		static void SetSpotLightData(USpotLightComponent* Target, const FSpotLightData& Data);

	UFUNCTION(BlueprintPure, Category = "Rendering|Components|Light", meta = (DefaultToSelf = "Target"))
		static FSpotLightData GetSpotLightData(const USpotLightComponent* Target);

	UFUNCTION(BlueprintCallable, Category = "Rendering|Components|Light", meta = (DefaultToSelf = "Target"))
		static void SetRectLightData(URectLightComponent* Target, const FRectLightData& Data);

	UFUNCTION(BlueprintPure, Category = "Rendering|Components|Light", meta = (DefaultToSelf = "Target"))
		static FRectLightData GetRectLightData(const URectLightComponent* Target);
};