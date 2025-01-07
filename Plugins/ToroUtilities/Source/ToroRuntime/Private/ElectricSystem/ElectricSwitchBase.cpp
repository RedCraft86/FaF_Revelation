// Copyright (C) RedCraft86. All Rights Reserved.

#include "ElectricSystem/ElectricSwitchBase.h"

AElectricSwitchBase::AElectricSwitchBase() : bState(false), PowerValue(1)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
#if WITH_EDITORONLY_DATA
	SceneRoot->bVisualizeComponent = true;
#endif
	SetRootComponent(SceneRoot);
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
	const FName ThisName = GetFName();
	for (const TObjectPtr<AElectricActorBase> Target : Targets)
	{
		bState ? Target->AddEnergy(ThisName, PowerValue) : Target->RemoveEnergy(ThisName);
	}

	StateChangedEvent(bState);
	OnStateChangedBP.Broadcast(bState);
}

void AElectricSwitchBase::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AElectricSwitchBase::UpdateStates, 0.1f, false);
}

void AElectricSwitchBase::OnEnableStateChanged(const bool bIsEnabled)
{
	SetActorEnableCollision(bIsEnabled);
	SetActorTickEnabled(PrimaryActorTick.bStartWithTickEnabled && bIsEnabled);
}
