// Copyright (C) RedCraft86. All Rights Reserved.

#include "DataTypes/LightingData.h"
#include "Components/LightComponent.h"
#include "Components/LocalLightComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/RectLightComponent.h"
#include "Components/SpotLightComponent.h"

FBaseLightData FBaseLightData::GetBaseLightData() const
{
	FBaseLightData Data;
	Data.IntensityUnits = IntensityUnits;
	Data.Intensity = Intensity;
	Data.Color = Color;
	Data.AttenuationRadius = AttenuationRadius;
	Data.bUseTemperature = bUseTemperature;
	Data.Temperature = Temperature;
	Data.IndirectIntensity = IndirectIntensity;
	Data.VolumetricScatteringIntensity = VolumetricScatteringIntensity;
	Data.bLightCastShadows = bLightCastShadows;
	Data.bCastVolumetricShadows = bCastVolumetricShadows;
	Data.SpecularScale = SpecularScale;
	Data.DrawDistance = DrawDistance;

	return Data;
}

void FBaseLightData::CopyFrom(const FBaseLightData& Data)
{
	IntensityUnits = Data.IntensityUnits;
	Intensity = Data.Intensity;
	Color = Data.Color;
	AttenuationRadius = Data.AttenuationRadius;
	bUseTemperature = Data.bUseTemperature;
	Temperature = Data.Temperature;
	IndirectIntensity = Data.IndirectIntensity;
	VolumetricScatteringIntensity = Data.VolumetricScatteringIntensity;
	bLightCastShadows = Data.bLightCastShadows;
	bCastVolumetricShadows = Data.bCastVolumetricShadows;
	SpecularScale = Data.SpecularScale;
	DrawDistance = Data.DrawDistance;
}

void FBaseLightData::CopyBaseData(const FBaseLightData& Data)
{
	IntensityUnits = Data.IntensityUnits;
	Intensity = Data.Intensity;
	Color = Data.Color;
	bUseTemperature = Data.bUseTemperature;
	Temperature = Data.Temperature;
}

FBaseLightData ULightingDataLibrary::GetBaseData_Point(const FPointLightData& Target)
{
	return Target.GetBaseLightData();
}

FBaseLightData ULightingDataLibrary::GetBaseData_Spot(const FSpotLightData& Target)
{
	return Target.GetBaseLightData();
}

FBaseLightData ULightingDataLibrary::GetBaseData_Rect(const FRectLightData& Target)
{
	return Target.GetBaseLightData();
}

FPointLightData ULightingDataLibrary::GetPointData(const FBaseLightData& Target, const bool bOnlyBasicData)
{
	FPointLightData Result;
	if (bOnlyBasicData) Result.CopyBaseData(Target); else Result.CopyFrom(Target);
	return Result;
}

FSpotLightData ULightingDataLibrary::GetSpotData(const FBaseLightData& Target, const bool bOnlyBasicData)
{
	FSpotLightData Result;
	if (bOnlyBasicData) Result.CopyBaseData(Target); else Result.CopyFrom(Target);
	return Result;
}

FRectLightData ULightingDataLibrary::GetRectData(const FBaseLightData& Target, const bool bOnlyBasicData)
{
	FRectLightData Result;
	if (bOnlyBasicData) Result.CopyBaseData(Target); else Result.CopyFrom(Target);
	return Result;
}

void ULightingDataLibrary::SetLightDrawDistanceSettings(ULightComponent* Target, const FLightDrawDistance& Settings)
{
	if (IsValid(Target))
	{
		Target->MaxDrawDistance = Settings.GetMaxDrawDistance();
		Target->MaxDistanceFadeRange = Settings.GetMaxDistanceFadeRange();
	}
}

FLightDrawDistance ULightingDataLibrary::GetLightDrawDistanceSettings(const ULightComponent* Target)
{
	FLightDrawDistance Result;
	if (IsValid(Target))
	{
		Result.MaxDrawDistance = Target->MaxDrawDistance;
		Result.MaxDistanceFadeRange = Target->MaxDistanceFadeRange;
		Result.bUseDrawDistance = Result.IsValidData();
	}

	return Result;
}

void ULightingDataLibrary::SetBaseLightData(ULightComponent* Target, const FBaseLightData& Data)
{
	if (IsValid(Target))
	{
		Target->SetIntensity(Data.Intensity);
		Target->SetLightColor(Data.Color);
		Target->SetUseTemperature(Data.bUseTemperature);
		Target->SetTemperature(Data.Temperature);
		Target->SetIndirectLightingIntensity(Data.IndirectIntensity);
		Target->SetVolumetricScatteringIntensity(Data.VolumetricScatteringIntensity);
		Target->SetCastShadows(Data.bLightCastShadows);
		Target->SetCastVolumetricShadow(Data.bCastVolumetricShadows);
		Target->SetSpecularScale(Data.SpecularScale);

		// NOTE: Intensity Units and Attenuation Radius are not part of the ULightComponent so we need to cast to ULocalLightComponent
		if (ULocalLightComponent* LocalLight = Cast<ULocalLightComponent>(Target))
		{
			LocalLight->SetIntensityUnits(Data.IntensityUnits);
			LocalLight->SetAttenuationRadius(Data.AttenuationRadius);
		}

		SetLightDrawDistanceSettings(Target, Data.DrawDistance);
	}
}

FBaseLightData ULightingDataLibrary::GetBaseLightData(const ULightComponent* Target)
{
	FBaseLightData OutData;
	if (IsValid(Target))
	{
		OutData.Intensity = Target->Intensity;
		OutData.Color = Target->LightColor;
		OutData.bUseTemperature = Target->bUseTemperature;
		OutData.Temperature = Target->Temperature;
		OutData.IndirectIntensity = Target->IndirectLightingIntensity;
		OutData.VolumetricScatteringIntensity = Target->VolumetricScatteringIntensity;
		OutData.bLightCastShadows = Target->CastShadows;
		OutData.bCastVolumetricShadows = Target->bCastVolumetricShadow;
		OutData.SpecularScale = Target->SpecularScale;

		// NOTE: Intensity Units and Attenuation Radius are not part of the ULightComponent so we need to cast to ULocalLightComponent
		if (const ULocalLightComponent* LocalLight = Cast<ULocalLightComponent>(Target))
		{
			OutData.IntensityUnits = LocalLight->IntensityUnits;
			OutData.AttenuationRadius = LocalLight->AttenuationRadius;
		}

		OutData.DrawDistance = GetLightDrawDistanceSettings(Target);
	}
	
	return OutData;
}

void ULightingDataLibrary::SetPointLightData(UPointLightComponent* Target, const FPointLightData& Data)
{
	if (IsValid(Target))
	{
		SetBaseLightData(Target, Data);
		Target->SetSourceRadius(Data.SourceRadius);
		Target->SetSoftSourceRadius(Data.SoftSourceRadius);
		Target->SetSourceLength(Data.SourceLength);
		Target->bUseInverseSquaredFalloff = Data.bUseInverseSquaredFalloff;
		Target->SetLightFalloffExponent(Data.LightFalloffExponent);
	}
}

FPointLightData ULightingDataLibrary::GetPointLightData(const UPointLightComponent* Target)
{
	FPointLightData OutData;
	if (Target)
	{
		OutData.CopyFrom(GetBaseLightData(Target));
		OutData.SourceRadius = Target->SourceRadius;
		OutData.SoftSourceRadius = Target->SoftSourceRadius;
		OutData.SourceLength = Target->SourceLength;
		OutData.bUseInverseSquaredFalloff = Target->bUseInverseSquaredFalloff;
		OutData.LightFalloffExponent = Target->LightFalloffExponent;
	}
	
	return OutData;
}

void ULightingDataLibrary::SetSpotLightData(USpotLightComponent* Target, const FSpotLightData& Data)
{
	if (IsValid(Target))
	{
		SetBaseLightData(Target, Data);
		Target->SetInnerConeAngle(Data.InnerConeAngle);
		Target->SetOuterConeAngle(Data.OuterConeAngle);
		Target->SetSourceRadius(Data.SourceRadius);
		Target->SetSoftSourceRadius(Data.SoftSourceRadius);
		Target->SetSourceLength(Data.SourceLength);
		Target->bUseInverseSquaredFalloff = Data.bUseInverseSquaredFalloff;
		Target->SetLightFalloffExponent(Data.LightFalloffExponent);
	}
}

FSpotLightData ULightingDataLibrary::GetSpotLightData(const USpotLightComponent* Target)
{
	FSpotLightData OutData;
	if (Target)
	{
		OutData.CopyFrom(GetBaseLightData(Target));
		OutData.InnerConeAngle = Target->InnerConeAngle;
		OutData.OuterConeAngle = Target->OuterConeAngle;
		OutData.SourceRadius = Target->SourceRadius;
		OutData.SoftSourceRadius = Target->SoftSourceRadius;
		OutData.SourceLength = Target->SourceLength;
		OutData.bUseInverseSquaredFalloff = Target->bUseInverseSquaredFalloff;
		OutData.LightFalloffExponent = Target->LightFalloffExponent;
	}

	return OutData;
}

void ULightingDataLibrary::SetRectLightData(URectLightComponent* Target, const FRectLightData& Data)
{
	if (IsValid(Target))
	{
		SetBaseLightData(Target, Data);
		Target->SetSourceWidth(Data.SourceWidth);
		Target->SetSourceHeight(Data.SourceHeight);
		Target->SetBarnDoorAngle(Data.BarnDoorAngle);
		Target->SetBarnDoorLength(Data.BarnDoorLength);
		Target->SetSourceTexture(Data.SourceTexture);
	}
}

FRectLightData ULightingDataLibrary::GetRectLightData(const URectLightComponent* Target)
{
	FRectLightData OutData;
	if (Target)
	{
		OutData.CopyFrom(GetBaseLightData(Target));
		OutData.SourceWidth = Target->SourceWidth;
		OutData.SourceHeight = Target->SourceHeight;
		OutData.BarnDoorAngle = Target->BarnDoorAngle;
		OutData.BarnDoorLength = Target->BarnDoorLength;
		OutData.SourceTexture = Target->SourceTexture;
	}
	
	return OutData;
}