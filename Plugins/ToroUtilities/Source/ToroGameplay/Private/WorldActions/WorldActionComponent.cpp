// Copyright (C) RedCraft86. All Rights Reserved.

#include "WorldActions/WorldActionComponent.h"

UWorldActionComponent::UWorldActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UWorldActionComponent::RunEvents()
{
	FOR_EACH_ACTION(Actions, {
		ActionPtr->RunEvent(this);
	})
}

void UWorldActionComponent::BeginPlay()
{
	Super::BeginPlay();
	FOR_EACH_ACTION(Actions, {
		ActionPtr->OnBeginPlay(this);
	})
}

void UWorldActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FOR_EACH_ACTION(Actions, {
		if (ActionPtr->bCanEverTick) ActionPtr->OnTick(this, DeltaTime);
	})
}

#if WITH_EDITOR
void UWorldActionComponent::PostLoad()
{
	Super::PostLoad();
	FOR_EACH_ACTION(Actions, {
		ActionPtr->OnPostEditChange(this);
		if (!PrimaryComponentTick.bStartWithTickEnabled)
		{
			PrimaryComponentTick.bStartWithTickEnabled = ActionPtr->bCanEverTick;
		}
	})
}

void UWorldActionComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	FOR_EACH_ACTION(Actions, {
		ActionPtr->OnPostEditChange(this);
		if (!PrimaryComponentTick.bStartWithTickEnabled)
		{
			PrimaryComponentTick.bStartWithTickEnabled = ActionPtr->bCanEverTick;
		}
	})
}
#endif
