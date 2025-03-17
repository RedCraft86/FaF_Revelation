// Copyright (C) RedCraft86. All Rights Reserved.

#include "ElectricSystem/ElectricWorldActions.h"

AElectricWorldActions::AElectricWorldActions()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AElectricWorldActions::BeginPlay()
{
	Super::BeginPlay();
	FOR_EACH_ACTION(Activate, {
		ActionPtr->OnBeginPlay(this);
	})
	FOR_EACH_ACTION(Deactivate, {
		ActionPtr->OnBeginPlay(this);
	})
}

void AElectricWorldActions::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	FOR_EACH_ACTION(Activate, {
		if (ActionPtr->bCanEverTick) ActionPtr->OnTick(this, DeltaSeconds);
	})
	FOR_EACH_ACTION(Deactivate, {
		if (ActionPtr->bCanEverTick) ActionPtr->OnTick(this, DeltaSeconds);
	})
}

void AElectricWorldActions::OnStateChanged(const bool bState)
{
	Super::OnStateChanged(bState);
	FOR_EACH_ACTION(Activate, {
		ActionPtr->RunEvent(this);
	})
	FOR_EACH_ACTION(Deactivate, {
		ActionPtr->RunEvent(this);
	})
}

#if WITH_EDITOR
void AElectricWorldActions::PostLoad()
{
	Super::PostLoad();
	FOR_EACH_ACTION(Activate, {
		ActionPtr->OnPostEditChange(this);
		if (!PrimaryActorTick.bStartWithTickEnabled)
		{
			PrimaryActorTick.bStartWithTickEnabled = ActionPtr->bCanEverTick;
		}
	})
	FOR_EACH_ACTION(Deactivate, {
		ActionPtr->OnPostEditChange(this);
		if (!PrimaryActorTick.bStartWithTickEnabled)
		{
			PrimaryActorTick.bStartWithTickEnabled = ActionPtr->bCanEverTick;
		}
	})
}

void AElectricWorldActions::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	FOR_EACH_ACTION(Activate, {
		ActionPtr->OnPostEditChange(this);
		if (!PrimaryActorTick.bStartWithTickEnabled)
		{
			PrimaryActorTick.bStartWithTickEnabled = ActionPtr->bCanEverTick;
		}
	})
	FOR_EACH_ACTION(Deactivate, {
		ActionPtr->OnPostEditChange(this);
		if (!PrimaryActorTick.bStartWithTickEnabled)
		{
			PrimaryActorTick.bStartWithTickEnabled = ActionPtr->bCanEverTick;
		}
	})
}
#endif
