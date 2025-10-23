// Copyright (C) RedCraft86. All Rights Reserved.

#include "QTEManager.h"

const UQTEInstance* UQTEManager::InitiateQuicktime(const TSubclassOf<UQTEInstance> Class)
{
	if (!Class || IsValid(ActiveQTE))
	{
		return nullptr;
	}

	if (ActiveQTE = NewObject<UQTEInstance>(this, Class); ActiveQTE)
	{
		ActiveQTE->OnFinished.BindUObject(this, &UQTEManager::QuicktimeFinished);
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
