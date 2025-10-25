// Copyright (C) RedCraft86. All Rights Reserved.

#include "QTEManager.h"

UQTEManager::UQTEManager()
{
	PrimaryComponentTick.TickGroup = TG_DuringPhysics;
	PrimaryComponentTick.bTickEvenWhenPaused = true;
}

const UQTEInstance* UQTEManager::InitiateEvent(const TSubclassOf<UQTEInstance> Class)
{
	if (!Class || IsValid(ActiveQTE))
	{
		return nullptr;
	}

	if (ActiveQTE = NewObject<UQTEInstance>(this, Class); ActiveQTE)
	{
		ActiveQTE->OnFinished.BindUObject(this, &UQTEManager::QuicktimeFinished);
		ActiveQTE->BeginQTE(GetWorld());
	}

	return ActiveQTE;
}

void UQTEManager::QuicktimeFinished(const bool bSuccess)
{
	if (ActiveQTE)
	{
		OnQuicktimeFinished.Broadcast(bSuccess, ActiveQTE->GetClass());
		OnQuicktimeFinishedBP.Broadcast(bSuccess, ActiveQTE->GetClass());
		ActiveQTE->MarkAsGarbage();
		ActiveQTE = nullptr;
	}
}

void UQTEManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (ActiveQTE) ActiveQTE->TickQTE(DeltaTime);
}
