// Copyright (C) RedCraft86. All Rights Reserved.

#include "ElectricSystem/ElectricLightBase.h"
#include "Components/LightComponent.h"

AElectricLightBase::AElectricLightBase() : FlickerRate(0.25f), FlickerRange(0.0f, 1.0f)
	, MeshMulti(1.0f), MeshFresnel(0.5f), MeshFlicker(0.0f, 1.0f)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	ZoneCulling = CreateDefaultSubobject<UZoneCullingComponent>("ZoneCulling");

	MinEnergy = 0;
	
	if (FRichCurve* Curve = FlickerCurve.GetRichCurve())
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

void AElectricLightBase::SetFlickerState(const bool bNewFlicker)
{
	if (bFlicker != bNewFlicker)
	{
		bFlicker = bNewFlicker;
		SetActorTickEnabled(ShouldTick());

		// Reset multipliers
		for (const FElectricLightEntry& Entry : CachedEntries)
		{
			if (Entry.Light) Entry.Light->SetIntensity(Entry.Intensity);
			for (const TPair<TObjectPtr<UStaticMeshComponent>, bool>& Mesh : Entry.Meshes)
			{
#if WITH_EDITOR
				if (!FApp::IsGame())
				{
					Mesh.Key->SetDefaultCustomPrimitiveDataFloat(5, MeshMulti);
				}
				else
#endif
				{
					Mesh.Key->SetCustomPrimitiveDataFloat(5, MeshMulti);
				}
			}
		}
	}
}

void AElectricLightBase::UpdateCaches()
{
	GetLightInfo(CachedEntries);
	for (const FElectricLightEntry& Entry : CachedEntries)
	{
		if (!Entry.Light) continue;
		for (const TPair<TObjectPtr<UStaticMeshComponent>, bool>& Mesh : Entry.Meshes)
		{
			if (!Mesh.Key) continue;
			FLinearColor Color = Entry.Light->GetLightColor();
			if (Entry.Light->bUseTemperature)
				Color *= FLinearColor::MakeFromColorTemperature(Entry.Light->Temperature);

			Color.A = Entry.Intensity;
			
#if WITH_EDITOR
			if (!FApp::IsGame())
			{
				Mesh.Key->SetDefaultCustomPrimitiveDataVector4(0, Color);
				Mesh.Key->SetDefaultCustomPrimitiveDataFloat(4, MeshFresnel);
				Mesh.Key->SetDefaultCustomPrimitiveDataFloat(5, MeshMulti);
			}
			else
#endif
			{
				Mesh.Key->SetCustomPrimitiveDataVector4(0, Color);
				Mesh.Key->SetCustomPrimitiveDataFloat(4, MeshFresnel);
				Mesh.Key->SetCustomPrimitiveDataFloat(5, MeshMulti);
			}
		}
	}
	
	if (bEnabled)
	{
		FlickerCurve.GetValueRange(FlickerValRange.X, FlickerValRange.Y);
		FlickerCurve.GetTimeRange(FlickerTimeRange.X, FlickerTimeRange.Y);
	}
	else FlickerValRange = FlickerTimeRange = {0.0f, 0.0f};
}

bool AElectricLightBase::ShouldTick() const
{
	return IsEnabled() && bCachedState && (bFlicker || WantsTick());
}

void AElectricLightBase::OnStateChanged(const bool bState)
{
	Super::OnStateChanged(bState);
	SetActorTickEnabled(ShouldTick());
	for (const FElectricLightEntry& Entry : CachedEntries)
	{
		if (Entry.Light) Entry.Light->SetVisibility(bState);
		for (const TPair<TObjectPtr<UStaticMeshComponent>, bool>& Mesh : Entry.Meshes)
		{
			if (!Mesh.Key) continue;
			if (Mesh.Value)
			{
				Mesh.Key->SetVisibility(bState);
			}
			else
			{
				Mesh.Key->SetVisibility(true);
#if WITH_EDITOR
				if (!FApp::IsGame())
					Mesh.Key->SetDefaultCustomPrimitiveDataFloat(3, bState ? Entry.Intensity : 0.0f);
				else
#endif
					Mesh.Key->SetCustomPrimitiveDataFloat(3, bState ? Entry.Intensity : 0.0f);
			}
		}
	}
}

void AElectricLightBase::OnEnableStateChanged(const bool bIsEnabled)
{
	Super::OnEnableStateChanged(bIsEnabled);
	SetActorTickEnabled(ShouldTick());
}

void AElectricLightBase::BeginPlay()
{
	UpdateCaches();
	Super::BeginPlay();
	SetActorTickEnabled(ShouldTick());
}

#define GET_MAPPED_FLICKER(TargetRange) FMath::Max(0.0f,FMath::GetMappedRangeValueClamped(FlickerValRange, TargetRange, Value))
void AElectricLightBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
#if WITH_EDITOR
	if ((!FApp::IsGame() || !IsHidden()) && IsEnabled() && GetState() && bFlicker)
#else
	if (!IsHidden() && IsEnabled() && bCachedState && bFlicker)
#endif
	{
		FlickerTime = FlickerTime + DeltaSeconds * FlickerRate;
		if (FlickerTime > FlickerTimeRange.Y) FlickerTime = FlickerTimeRange.X;

		const float Value = FlickerCurve.GetValue(FlickerTime);
#if WITH_EDITORONLY_DATA
		FlickerPlayback = GET_MAPPED_FLICKER(FlickerRange);
#endif
		for (const FElectricLightEntry& Entry : CachedEntries)
		{
			if (!Entry.Light) continue;
			Entry.Light->SetIntensity(Entry.Intensity * GET_MAPPED_FLICKER(FlickerRange));
			for (const TPair<TObjectPtr<UStaticMeshComponent>, bool>& Mesh : Entry.Meshes)
			{
				if (Mesh.Key)
				{
#if WITH_EDITOR
					if (!FApp::IsGame())
					{
						Mesh.Key->SetDefaultCustomPrimitiveDataFloat(5, MeshMulti
							* GET_MAPPED_FLICKER(bMeshFlicker ? MeshFlicker : FlickerRange));
					}
					else
#endif
						Mesh.Key->SetCustomPrimitiveDataFloat(5, MeshMulti
							* GET_MAPPED_FLICKER(bMeshFlicker ? MeshFlicker : FlickerRange));
				}
			}
		}
	}
}
#undef GET_MAPPED_FLICKER
#if WITH_EDITOR
void AElectricLightBase::OnConstruction(const FTransform& Transform)
{
	if (!FApp::IsGame()) UpdateCaches();
	Super::OnConstruction(Transform);
}
#endif