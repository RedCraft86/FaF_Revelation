// Copyright (C) RedCraft86. All Rights Reserved.

#include "SaveSystem/ToroSaveManager.h"
#include "ToroSettings.h"

UToroSaveObject* UToroSaveManager::GetSaveObject(const FGameplayTag InTag)
{
	if (VerifySaveTag(InTag)) return nullptr;
	if (TObjectPtr<UToroSaveObject>* Ptr = SaveObjects.Find(InTag))
	{
		if (*Ptr) return *Ptr;
		SaveObjects.Remove(InTag);
	}

	return nullptr;
}

void UToroSaveManager::OnSaveActivity(const UToroSaveObject* Save, const ESaveGameActivity Type) const
{
	OnSaveIO.Broadcast(Save, Type);
}

void UToroSaveManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	const UToroSettings* Settings = UToroSettings::Get();
	for (const TPair<FGameplayTag, TSoftClassPtr<UToroSaveObject>>& Info : Settings->SaveObjects)
	{
		if (UToroSaveObject* Obj = UToroSaveObject::Create(this, Info.Value.LoadSynchronous(), Info.Key))
		{
			SaveObjects.Add(Info.Key, Obj);
			Obj->OnSaveActivity.AddUObject(this, &UToroSaveManager::OnSaveActivity);
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
