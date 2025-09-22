// Copyright (C) RedCraft86. All Rights Reserved.

#include "SaveSystem/ToroSaveManager.h"

void FSaveSlots::AddSlotObject(UToroSaveObject* Object)
{
	if (Object)
	{
		Slots.Add(Object->GetSaveSlot(), Object);
	}
}

UToroSaveObject* FSaveSlots::GetSlotObject(const uint8 Slot)
{
	TObjectPtr<UToroSaveObject>* Obj = Slots.Find(Slot);
	return Obj ? *Obj : nullptr;
}

UToroSaveObject* UToroSaveManager::FindOrAddSave(const TSubclassOf<UToroSaveObject> SaveClass, const uint8 Slot)
{
	if (!SaveClass)
	{
		return nullptr;
	}

	FSaveSlots* Slots = SaveObjects.Find(SaveClass);
	if (UToroSaveObject* ExistingObj = Slots ? Slots->GetSlotObject(Slot) : nullptr)
	{
		return ExistingObj;
	}

	if (UToroSaveObject* NewObj = UToroSaveObject::Create(this, SaveClass, Slot))
	{
		SaveObjects.FindOrAdd(SaveClass).AddSlotObject(NewObj);
		return NewObj;
	}
    
	return nullptr;
}

void UToroSaveManager::OnActivity(const UToroSaveObject* Save, const ESaveGameActivity Activity) const
{
	OnSaveActivity.Broadcast(Save, Activity);
}

void UToroSaveManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// TODO automatic loading
}

void UToroSaveManager::Deinitialize()
{
	Super::Deinitialize();
	for (const TPair<TSubclassOf<UToroSaveObject>, FSaveSlots>& Types : SaveObjects)
	{
		for (const TPair<uint8, TObjectPtr<UToroSaveObject>>& Slot : Types.Value.Slots)
		{
			Slot.Value->SaveObject(nullptr);
		}
	}
}
