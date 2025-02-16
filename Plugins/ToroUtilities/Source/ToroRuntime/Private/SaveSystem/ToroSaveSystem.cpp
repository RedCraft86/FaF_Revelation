// Copyright (C) RedCraft86. All Rights Reserved.

#include "SaveSystem/ToroSaveSystem.h"
#include "ToroRuntimeSettings.h"
#include "ToroRuntime.h"

UToroSaveObject* UToroSaveSystem::GetSaveObject(const FGameplayTag InTag)
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

void UToroSaveSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	const UToroRuntimeSettings* Settings = UToroRuntimeSettings::Get();
	for (const TPair<FGameplayTag, TSoftClassPtr<UToroSaveObject>>& Info : Settings->SaveObjects)
	{
		if (UToroSaveObject* Obj = UToroSaveObject::Create(this, Info.Value.LoadSynchronous(), Info.Key))
		{
			SaveObjects.Add(Info.Key, Obj);
		}
	}
}

void UToroSaveSystem::Deinitialize()
{
	for (const TPair<FGameplayTag, TObjectPtr<UToroSaveObject>>& Obj : SaveObjects)
	{
		if (Obj.Value) Obj.Value->SaveObject(nullptr);
	}
	Super::Deinitialize();
}
