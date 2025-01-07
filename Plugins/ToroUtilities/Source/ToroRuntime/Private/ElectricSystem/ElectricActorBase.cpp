// Copyright (C) RedCraft86. All Rights Reserved.

#include "ElectricSystem/ElectricActorBase.h"

AElectricActorBase::AElectricActorBase() : MinEnergy(1), bRequiresCollision(false), bCachedState(false)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AElectricActorBase::AddEnergy(const FName Key, const uint8 Value)
{
	if (Value > 0 && !Key.IsNone())
	{
		Energy.Add(Key, Value);
		CachedEnergy.Reset();
		OnEnergyChanged(GetEnergy());
	}
}

void AElectricActorBase::RemoveEnergy(const FName Key)
{
	if (Energy.Contains(Key))
	{
		Energy.Remove(Key);
		CachedEnergy.Reset();
		OnEnergyChanged(GetEnergy());
	}
}

uint8 AElectricActorBase::GetEnergy()
{
	if (CachedEnergy.IsSet())
		return CachedEnergy.GetValue();
	
	uint8 Out = 0;
	for (const TPair<FName, uint8>& Pair : Energy)
	{
		if (Out == 255) break;
		Out += Pair.Value;
	}

	CachedEnergy = Out;
	return Out;
}

bool AElectricActorBase::GetState()
{
#if WITH_EDITOR
	return FApp::IsGame() ? GetEnergy() >= MinEnergy : bPreviewState;
#else
	return GetEnergy() >= MinEnergy;
#endif
}

void AElectricActorBase::OnEnergyChanged(const uint8 Total)
{
	const bool bState = GetState();
	if (bCachedState != bState)
	{
		bCachedState = bState;
		OnStateChanged(bCachedState);
	}
	
	EnergyChangedEvent(Total);
}

void AElectricActorBase::OnStateChanged(const bool bInState)
{
	StateChangedEvent(bInState);
	OnStateChangedBP.Broadcast(bInState);
}

void AElectricActorBase::BeginPlay()
{
	Super::BeginPlay();
	bCachedState = GetState();
	OnStateChanged(bCachedState);
	SetActorEnableCollision(bRequiresCollision);
}

#if WITH_EDITOR
void AElectricActorBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (!FApp::IsGame())
	{
		bCachedState = GetState();
		OnStateChanged(bCachedState);
	}
}
#endif
