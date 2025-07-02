// Copyright (C) RedCraft86. All Rights Reserved.

#include "Lighting/ToroLightBase.h"

AToroLightBase::AToroLightBase(): bReferenceCull(true), FlickerType(0), Multiplier(1.0f), Fresnel(0.5f)
{
	PrimaryActorTick.TickGroup = TG_DuringPhysics;

	RefCulling = CreateDefaultSubobject<URefCullComponent>("RefCulling");
	RefCulling->bAffectTicking = false;
}

void AToroLightBase::SetFlickerType(const uint8 InFlicker)
{
	if (FlickerType != InFlicker)
	{
		FlickerType = FMath::Clamp(InFlicker, 0, FlickerMats.Num());
		UpdateLights();
	}
}

void AToroLightBase::UpdateLights()
{
	ApplyLightSettings();
	UMaterialInterface* FlickerMat = FlickerType == 0 ? nullptr : FlickerMats[FlickerType - 1];
	for (TArray<FToroLightEntry> Entries = GetLights(); const FToroLightEntry& Entry : Entries)
	{
		Entry.UpdateLight(IsEnabled(), Multiplier, Fresnel, FlickerType, FlickerMat);
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
	FlickerType = FMath::Clamp(FlickerType, 0, FlickerMats.Num());
	UpdateLights();
}
