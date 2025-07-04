// Copyright (C) RedCraft86. All Rights Reserved.

#include "Lighting/ToroLightBase.h"

AToroLightBase::AToroLightBase(): bReferenceCull(true), Multiplier(1.0f), Fresnel(0.5f)
{
	PrimaryActorTick.TickGroup = TG_DuringPhysics;

	RefCulling = CreateDefaultSubobject<URefCullComponent>("RefCulling");
	RefCulling->bAffectTicking = false;
}

void AToroLightBase::SetFlickerMat(UMaterialInterface* InMaterial)
{
	if (FlickerMat != InMaterial)
	{
		FlickerMat = InMaterial;
		UpdateLights();
	}
}

void AToroLightBase::UpdateLights() const
{
	ApplyLightSettings();
	for (TArray<FToroLightEntry> Entries = GetLights(); const FToroLightEntry& Entry : Entries)
	{
		Entry.UpdateLight(IsEnabled(), Multiplier, Fresnel, FlickerMat);
	}
}

void AToroLightBase::EnableStateChanged(const bool bIsEnabled)
{
	Super::EnableStateChanged(bIsEnabled);
	UpdateLights();
}

void AToroLightBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	UpdateLights();
}
