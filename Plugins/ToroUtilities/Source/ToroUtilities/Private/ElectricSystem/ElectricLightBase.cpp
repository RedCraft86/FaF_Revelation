// Copyright (C) RedCraft86. All Rights Reserved.

// ReSharper disable CppExpressionWithoutSideEffects
#include "ElectricSystem/ElectricLightBase.h"
#include "Components/LightComponent.h"

#define INTENSITY_INDEX 3
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

void FElectricLightEntry::ForEachMesh(const TFunctionRef<void(UStaticMeshComponent*, const bool)>& Func) const
{
	for (const TPair<TObjectPtr<UStaticMeshComponent>, bool>& Mesh : Meshes)
	{
		if (Mesh.Key)
		{
			Func(Mesh.Key, Mesh.Value);
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
	
	ForEachMesh([this, MeshFresnel](UStaticMeshComponent* Mesh, const bool)
	{
		SetPrimitiveDataColor(Mesh, MeshColor);
		SetPrimitiveDataFloat(Mesh, FRESNEL_INDEX, MeshFresnel);
	});
}

FElectricLightAnim::FElectricLightAnim() : bEnabled(false), PlayRate(1.0f), AnimTime(0.0f)
	, bLightRange(false), LightRange(0.0f, 1.0f), bMeshRange(false), MeshRange(0.0f, 1.0f)
	, bFinished(false), bPlaying(false), TimeRange(0.0f), AlphaRange(0.0f)
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
		return;
	}
	
	AnimTime = TimeRange.X;
	bFinished = false;
	bPlaying = true;
}

void FElectricLightAnim::Stop(const bool bMarkAsDone)
{
	if (!bEnabled || !bPlaying) return;
	
	bPlaying = false;
	AnimTime = TimeRange.X;
	if (bMarkAsDone) bFinished = true;
}

void FElectricLightAnim::OnTick(const float DeltaTime)
{
	if (!bEnabled || !bPlaying) return;
	AnimTime += DeltaTime * PlayRate;
	if (AnimTime > TimeRange.Y)
	{
		if (OnAnimFinished.IsBound())
		{
			bPlaying = false;
			bFinished = true;
			AnimTime = TimeRange.X;
			OnAnimFinished.ExecuteIfBound();
		}
		else
		{
			AnimTime = TimeRange.X;
		}
	}
}

#define GET_MAPPED_ALPHA(RemapRange) FMath::GetMappedRangeValueClamped(AlphaRange, {(RemapRange).GetMin(), (RemapRange).GetMax()}, RawColor.A)
void FElectricLightAnim::ModifyValues(FLinearColor& Light, FLinearColor& Mesh) const
{
	if (!bEnabled || !bPlaying) return;
	const FLinearColor RawColor = AnimCurve.GetValue(AnimTime);

	Light.A *= bLightRange ? GET_MAPPED_ALPHA(LightRange) : RawColor.A;
	Light.R *= RawColor.R;
	Light.G *= RawColor.G;
	Light.B *= RawColor.B;

	Mesh.A *= bLightRange ? GET_MAPPED_ALPHA(bMeshRange ? MeshRange : LightRange) : RawColor.A;
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
	InAnim.PlayRate = 0.25f;
	if (FRichCurve* Curve = InAnim.AnimCurve.GetRichCurve(3))
	{
		Curve->UpdateOrAddKey(0.0f, 0.0f);
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

AElectricLightBase::AElectricLightBase() : MeshMulti(1.0f), MeshFresnel(0.5f), bReapplyState(true)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;
	
	SmartCulling = CreateDefaultSubobject<USmartCullingComponent>("SmartCulling");
	SmartCulling->bAffectTicking = true;
	
	MinEnergy = 0;
	FElectricLightAnim::SetFlickerCurve(FlickerAnim);
	
	FElectricLightAnim::SetWarmupCurve(WarmupAnim);
	WarmupAnim.OnAnimFinished.BindUObject(this, &AElectricLightBase::HandleLightAnims);
	
	FElectricLightAnim::SetBreakCurve(BreakAnim);
	BreakAnim.OnAnimFinished.BindUObject(this, &AElectricLightBase::ApplyLightState);
}

void AElectricLightBase::UpdateCaches()
{
	GetLights(LightEntries);
	for (int i = LightEntries.Num() - 1; i >= 0; i--)
	{
		if (LightEntries[i].Light)
		{
			LightEntries[i].UpdateCaches(MeshMulti, MeshFresnel);
		}
		else LightEntries.RemoveAt(i);
	}

	WarmupAnim.UpdateCaches();
	FlickerAnim.UpdateCaches();
	BreakAnim.UpdateCaches();
}

void AElectricLightBase::UpdateLights()
{
	ApplyLightState();
	GetLightState() ? HandleLightAnims() : StopAnimations(true);
}

void AElectricLightBase::ApplyLightState()
{
	const bool bState = GetLightState();
	for (const FElectricLightEntry& Entry : LightEntries)
	{
		Entry.Light->SetVisibility(bState);
		Entry.SetLightColorIntensity(Entry.LightColor);
		Entry.ForEachMesh([Entry, bState](UStaticMeshComponent* Mesh, const bool bShouldHide)
		{
			if (bShouldHide)
			{
				Mesh->SetVisibility(bState);
			}
			else
			{
				Mesh->SetVisibility(true);
				SetPrimitiveDataColor(Mesh, bState ? Entry.MeshColor : FLinearColor::Transparent);
			}
		});
	}

	if (!bState) WarmupAnim.bFinished = false;
}

bool AElectricLightBase::GetLightState() const
{
	return bCachedState && !(BreakStage == EElectricBreakStage::Broken && BreakAnim.bFinished);
}

void AElectricLightBase::StopAnimations(const bool bReset)
{
	WarmupAnim.Stop(false);
	if (bReset) WarmupAnim.bFinished = false;
	
	FlickerAnim.Stop(false);
	if (bReset) FlickerAnim.bFinished = false;
	
	BreakAnim.Stop(true);
}

void AElectricLightBase::HandleLightAnims()
{
	StopAnimations(false);

	if (LightTickType == EElectricLightTick::AlwaysOff)
	{
		WarmupAnim.bFinished = true;
		if (BreakStage == EElectricBreakStage::Broken)
		{
			BreakAnim.bFinished = true;
			ApplyLightState();
		}
		return;
	}

	if (WarmupAnim.bEnabled && !WarmupAnim.bFinished
		&& BreakStage != EElectricBreakStage::Broken)
	{
		WarmupAnim.Play();
		return;
	}

	switch (BreakStage)
	{
	case EElectricBreakStage::Working:
		break;
		
	case EElectricBreakStage::Breaking:
		FlickerAnim.Play();
		break;

	case EElectricBreakStage::Broken:
		BreakAnim.Play();
		break;
	}
}

bool AElectricLightBase::CanTick() const
{
	switch (LightTickType)
	{
	case EElectricLightTick::AlwaysOff: return false;
	case EElectricLightTick::AlwaysOn: return true;
	case EElectricLightTick::WithLightState: return bCachedState;
	}
	
	return false; // Should never reach this
}

bool AElectricLightBase::TickCondition()
{
#if WITH_EDITOR
	return (!FApp::IsGame() || !IsHidden()) && GetState() && IsPlayingAnimations();
#else
	return !IsHidden() && bCachedState && IsPlayingAnimations();
#endif
}

bool AElectricLightBase::IsPlayingAnimations() const
{
	return WarmupAnim.IsPlaying() || FlickerAnim.IsPlaying() || BreakAnim.IsPlaying();
}

void AElectricLightBase::OnStateChanged(const bool bState)
{
#if WITH_EDITOR
	if (!FApp::IsGame())
	{
		Super::OnStateChanged(bState);
		return;
	}
#endif
	SetActorTickEnabled(CanTick());
	Super::OnStateChanged(bState);
	UpdateLights();
}

void AElectricLightBase::OnBreakStageChanged(const EElectricBreakStage BreakState)
{
#if WITH_EDITOR
	if (!FApp::IsGame())
	{
		Super::OnBreakStageChanged(BreakState);
		return;
	}
#endif
	SetActorTickEnabled(CanTick());
	Super::OnBreakStageChanged(BreakState);
	if (BreakState != EElectricBreakStage::Broken) BreakAnim.bFinished = false;
	UpdateLights();
}

void AElectricLightBase::BeginPlay()
{
	UpdateCaches();
	Super::BeginPlay();
	SetActorTickEnabled(CanTick());
}

void AElectricLightBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (TickCondition())
	{
		bReapplyState = true;
		WarmupAnim.OnTick(DeltaSeconds);
		FlickerAnim.OnTick(DeltaSeconds);
		BreakAnim.OnTick(DeltaSeconds);
		for (const FElectricLightEntry& Entry : LightEntries)
		{
			FLinearColor LightColor = Entry.LightColor;
			FLinearColor MeshColor = Entry.MeshColor;
						
			WarmupAnim.ModifyValues(LightColor, MeshColor);
			FlickerAnim.ModifyValues(LightColor, MeshColor);
			BreakAnim.ModifyValues(LightColor, MeshColor);
			
			Entry.SetLightColorIntensity(LightColor);
			Entry.ForEachMesh([MeshColor](UStaticMeshComponent* Mesh, const bool)
			{
				SetPrimitiveDataColor(Mesh, MeshColor);
			});
		}
	}
	else if (bReapplyState)
	{
		bReapplyState = false;
		ApplyLightState();
	}
}

#if WITH_EDITOR
void AElectricLightBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (!FApp::IsGame())
	{
		BreakAnim.bFinished = false;
		ApplyLightSettings();
		UpdateCaches();
		UpdateLights();
	}
}
#endif
