﻿// Copyright (C) RedCraft86. All Rights Reserved.

#include "ElectricSwitch.h"
#include "ElectricActor.h"

AElectricSwitchBase::AElectricSwitchBase() : bState(false), PowerValue(1), bSingleUse(false)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AElectricSwitchBase::SetState(const bool bNewState)
{
	if (bState != bNewState)
	{
		bState = bNewState;
		UpdateStates();
	}
}

void AElectricSwitchBase::ToggleState()
{
	SetState(!bState);
}

void AElectricSwitchBase::UpdateStates()
{
	for (const TObjectPtr<AElectricActorBase> Target : Targets)
	{
		bState ? Target->AddEnergy(GetFName(), PowerValue) : Target->RemoveEnergy(GetFName());
	}

	StateChangedEvent(bState);
}

bool AElectricSwitchBase::GetInteractionInfo_Implementation(FInteractionInfo& Info)
{
	Info = InteractionInfo;
	return bEnabled;
}

void AElectricSwitchBase::OnBeginInteract_Implementation(AFRPlayerBase* Player, const FHitResult& HitResult)
{
	ToggleState();
}

void AElectricSwitchBase::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AElectricSwitchBase::UpdateStates, 0.1f, false);
}

void AElectricSwitchBase::OnEnableStateChanged(const bool bIsEnabled)
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(bIsEnabled);
	SetActorTickEnabled(PrimaryActorTick.bStartWithTickEnabled && bIsEnabled);
}
