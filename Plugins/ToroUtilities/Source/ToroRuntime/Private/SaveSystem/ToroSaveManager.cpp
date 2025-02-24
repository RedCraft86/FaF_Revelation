// Copyright (C) RedCraft86. All Rights Reserved.

#include "SaveSystem/ToroSaveManager.h"
#include "ToroRuntimeSettings.h"
#include "ToroRuntime.h"

UToroSaveObject* UToroSaveManager::GetSaveObject(const FGameplayTag InTag)
{
	if (!InTag.IsValid() || InTag == Tag_Saves) return nullptr;
	if (TObjectPtr<UToroSaveObject>* Ptr = SaveObjects.Find(InTag))
	{
		if (*Ptr) return *Ptr;
		SaveObjects.Remove(InTag);
	}

	UE_LOG(LogToroRuntime, Error, TEXT("Failed to find save object with tag: %s"), *InTag.ToString());
	return nullptr;
}

void UToroSaveManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	const TMap<FGameplayTag, TSoftClassPtr<UToroSaveObject>>& DefaultSaves = UToroRuntimeSettings::Get()->SaveObjects;
	for (const TPair<FGameplayTag, TSoftClassPtr<UToroSaveObject>>& Info : DefaultSaves)
	{
		if (UToroSaveObject* Obj = UToroSaveObject::Create(this, Info.Value.LoadSynchronous(), Info.Key))
		{
			SaveObjects.Add(Info.Key, Obj);
		}
	}
}

void UToroSaveManager::Deinitialize()
{
	for (const TPair<FGameplayTag, TObjectPtr<UToroSaveObject>>& Obj : SaveObjects)
	{
		if (Obj.Value) Obj.Value->SaveObject(nullptr);
	}
	Super::Deinitialize();
}
