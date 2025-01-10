// Copyright (C) RedCraft86. All Rights Reserved.

#include "ElectricSystem/ElectricLightBase.h"

FElectricLightAnim::FElectricLightAnim() : bEnabled(false), PlayRate(1.0f), IntensityRange(0.0f, 1.0f)
	, bPlaying(false), bLooping(false), AnimTime(0.0f), TimeRange(0.0f), AlphaRange(0.0f)
{
	for (uint8 i = 0; i < 4; i++)
	{
		if (FRichCurve* Curve = AnimCurve.GetRichCurve(i))
		{
			Curve->UpdateOrAddKey(0.0f, 1.0f);
			Curve->UpdateOrAddKey(0.5f, 1.0f);
		}
	}
}

void FElectricLightAnim::Stop()
{
	if (!bPlaying) return;
	bPlaying = false;
	bLooping = false;
	AnimTime = TimeRange.X;
}

void FElectricLightAnim::Play()
{
	if (bPlaying) return;
	AnimTime = TimeRange.X;
	bLooping = false;
	bPlaying = true;
}

void FElectricLightAnim::PlayLooping()
{
	if (bPlaying) return;
	AnimTime = TimeRange.X;
	bLooping = true;
	bPlaying = true;
}

void FElectricLightAnim::CacheValues()
{
	AnimCurve.GetTimeRange(TimeRange.X, TimeRange.Y);
	if (const FRichCurve* Curve = AnimCurve.GetRichCurveConst(3))
	{
		float X, Y;
		Curve->GetValueRange(X, Y);
		AlphaRange.X = X; AlphaRange.Y = Y;
	}
	
	AnimTime = TimeRange.X;
}

FLinearColor FElectricLightAnim::OnTick(const float DeltaTime)
{
	if (!bPlaying) return FLinearColor::White;

	AnimTime += DeltaTime * PlayRate;
	if (AnimTime > TimeRange.Y)
	{
		AnimTime = TimeRange.X;
		if (!bLooping)
		{
			bPlaying = false;
			return FLinearColor::White;
		}
	}
	
	FLinearColor Color = AnimCurve.GetValue(FMath::Clamp(AnimTime, TimeRange.X, TimeRange.Y));
	Color.A = FMath::Max(0.0f, FMath::GetMappedRangeValueClamped(AlphaRange, IntensityRange, Color.A));
	return Color;
}

void FElectricLightAnim::SetFlickerCurve(FElectricLightAnim& InAnim)
{
	if (FRichCurve* Curve = InAnim.AnimCurve.GetRichCurve(3))
	{
		Curve->UpdateOrAddKey(0.0f, 1.0f);
		Curve->UpdateOrAddKey(0.0f, 1.0f);
		Curve->UpdateOrAddKey(0.06f, 0.28f);
		Curve->UpdateOrAddKey(0.1f, 0.95f);
		Curve->UpdateOrAddKey(0.13f, 0.15f);
		Curve->UpdateOrAddKey(0.17f, 0.69f);
		Curve->UpdateOrAddKey(0.2f, 0.37f);
		Curve->UpdateOrAddKey(0.23f, 1.24f);
		Curve->UpdateOrAddKey(0.26f, 0.69f);
		Curve->UpdateOrAddKey(0.3f, 1.33f);
		Curve->UpdateOrAddKey(0.33f, 0.32f);
		Curve->UpdateOrAddKey(0.36f, 0.88f);
		Curve->UpdateOrAddKey(0.4f, 0.65f);
		Curve->UpdateOrAddKey(0.42f, 0.97f);
		Curve->UpdateOrAddKey(0.44f, 0.43f);
		Curve->UpdateOrAddKey(0.46f, 0.69f);
		Curve->UpdateOrAddKey(0.48f, 0.58f);
		Curve->UpdateOrAddKey(0.5f, 1.0f);
	}
}

AElectricLightBase::AElectricLightBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;
	
	ZoneCulling = CreateDefaultSubobject<UZoneCullingComponent>("ZoneCulling");

	MinEnergy = 0;

	FElectricLightAnim::SetFlickerCurve(FlickerAnim);
}
