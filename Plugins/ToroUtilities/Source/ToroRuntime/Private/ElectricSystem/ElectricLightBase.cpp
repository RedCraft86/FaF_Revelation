// Copyright (C) RedCraft86. All Rights Reserved.

// ReSharper disable CppExpressionWithoutSideEffects
#include "ElectricSystem/ElectricLightBase.h"
#include "Components/LightComponent.h"

AElectricLightBase::AElectricLightBase() : FlickerRate(0.25f), FlickerRange(0.0f, 1.0f)
	, MeshMulti(1.0f), MeshFresnel(0.5f), MeshFlicker(0.0f, 1.0f)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	ZoneCulling = CreateDefaultSubobject<UZoneCullingComponent>("ZoneCulling");

#define FRESNEL_INDEX 4

void SetPrimitiveDataFloat(UStaticMeshComponent* InComp, const int Idx, const float Value)
{
#if WITH_EDITOR
	if (!FApp::IsGame())
		InComp->SetDefaultCustomPrimitiveDataFloat(Idx, Value);
	else
#endif
		InComp->SetCustomPrimitiveDataFloat(Idx, Value);
}

void SetPrimitiveDataColor(UStaticMeshComponent* InComp, const FLinearColor& Value)
{
#if WITH_EDITOR
	if (!FApp::IsGame())
		InComp->SetDefaultCustomPrimitiveDataVector4(0, Value);
	else
#endif
		InComp->SetCustomPrimitiveDataVector4(0, Value);
}

void FElectricLightEntry::SetLightColorIntensity(const FLinearColor& InValue) const
{
	if (Light)
	{
		Light->SetIntensity(InValue.A);
		Light->SetLightColor({InValue.R, InValue.G, InValue.B});
	}
}

void FElectricLightEntry::ForEachMesh(const TFunctionRef<void(UStaticMeshComponent*)>& Func)
{
	for (const TPair<TObjectPtr<UStaticMeshComponent>, bool>& Mesh : Meshes)
	{
		if (Mesh.Key)
		{
			Func(Mesh.Key);
		}
	}
}

void FElectricLightEntry::UpdateCaches(const float MeshMulti, const float MeshFresnel)
{
	if (!Light) return;
	LightColor = Light->GetLightColor();
	LightColor.A = Light->Intensity;

	MeshColor = LightColor * Light->GetColorTemperature();
	MeshColor.A = LightColor.A * MeshMulti;
	
	ForEachMesh([this, MeshFresnel](UStaticMeshComponent* Mesh)
	{
		SetPrimitiveDataColor(Mesh, MeshColor);
		SetPrimitiveDataFloat(Mesh, FRESNEL_INDEX, MeshFresnel);
	});
}

FElectricLightAnim::FElectricLightAnim() : bEnabled(false), PlayRate(1.0f)
	, LightRange(0.0f, 1.0f), bMeshRange(false), MeshRange(0.0f, 1.0f)
	, AnimTime(0.0f), bFinished(false), bPlaying(false), TimeRange(0.0f), AlphaRange(0.0f)
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

void FElectricLightAnim::Play()
{
	if (bPlaying) return;
	if (!bEnabled)
	{
		bFinished = true;
		OnAnimFinished.ExecuteIfBound();
	}
	else
	{
		AnimTime = TimeRange.X;
		bFinished = false;
		bPlaying = true;
	}
}

void FElectricLightAnim::Stop(const bool bMarkAsDone)
{
	if (!bPlaying) return;
	if (bEnabled)
	{
		bPlaying = false;
		AnimTime = TimeRange.X;
		if (bMarkAsDone) bFinished = true;
	}
}

void FElectricLightAnim::Reset()
{
	bFinished = false;
	AnimTime = TimeRange.X;
}

void FElectricLightAnim::OnTick(const float DeltaTime)
{
	if (!bPlaying || !bEnabled) return;
	AnimTime += DeltaTime * PlayRate;
	if (AnimTime > TimeRange.Y)
	{
		if (OnAnimFinished.IsBound())
		{
			bPlaying = false;
			bFinished = true;
			OnAnimFinished.ExecuteIfBound();
		}
		else
		{
			AnimTime = TimeRange.X;
		}
	}
}

#define GET_MAPPED_ALPHA(RemapRange) FMath::GetMappedRangeValueClamped(AlphaRange, RemapRange, RawColor.A)
void FElectricLightAnim::ModifyValues(FLinearColor& Light, FLinearColor& Mesh) const
{
	const FLinearColor RawColor = bPlaying ? AnimCurve.GetValue(AnimTime) :
		bFinished ? AnimCurve.GetValue(TimeRange.Y) : FLinearColor::White;

	Light.A = GET_MAPPED_ALPHA(LightRange);
	Light.R *= RawColor.R;
	Light.G *= RawColor.G;
	Light.B *= RawColor.B;

	Mesh.A = GET_MAPPED_ALPHA(bMeshRange ? MeshRange : MeshRange);
	Mesh.R *= RawColor.R;
	Mesh.G *= RawColor.G;
	Mesh.B *= RawColor.B;
}
#undef GET_MAPPED_ALPHA

void FElectricLightAnim::UpdateCaches()
{
	AnimCurve.GetTimeRange(TimeRange.X, TimeRange.Y);
	if (const FRichCurve* Curve = AnimCurve.GetRichCurveConst(3))
	{
		float X, Y;
		Curve->GetValueRange(X, Y);
		AlphaRange.X = X; AlphaRange.Y = Y;
	}
}

void FElectricLightAnim::SetWarmupCurve(FElectricLightAnim& InAnim)
{
}

void FElectricLightAnim::SetFlickerCurve(FElectricLightAnim& InAnim)
{
	InAnim.bEnabled = true;
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

void FElectricLightAnim::SetBreakCurve(FElectricLightAnim& InAnim)
{
}
