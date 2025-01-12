// Copyright (C) RedCraft86. All Rights Reserved.

#include "ElectricSystem/ElectricActorBase.h"

AElectricActorBase::AElectricActorBase() : BreakStage(EElectricBreakStage::Working)
	, MinEnergy(1), bRequiresCollision(false), bCachedState(false)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);
	
#if WITH_EDITORONLY_DATA
	CREATE_DEBUG_ICON(DebugIcon)
#endif
}

void AElectricActorBase::SetBreakStage(const EElectricBreakStage InStage)
{
	if (BreakStage != InStage)
	{
		BreakStage = InStage;
		OnBreakStageChanged(InStage);
	}
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

void AElectricActorBase::OnBreakStageChanged(const EElectricBreakStage BreakState)
{
	BreakStageChangedEvent(BreakState);
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
	UPDATE_DEBUG_ICON(DebugIcon);
	if (!FApp::IsGame())
	{
		bCachedState = GetState();
		OnStateChanged(bCachedState);
	}
}

void AElectricActorBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, BreakStage))
	{
		OnBreakStageChanged(BreakStage);
	}
}
#endif
