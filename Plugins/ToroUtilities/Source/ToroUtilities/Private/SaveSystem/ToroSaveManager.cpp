// Copyright (C) RedCraft86. All Rights Reserved.

#include "SaveSystem/ToroSaveManager.h"
#include "ToroUtilities.h"

UToroSaveObject* UToroSaveManager::FindSaveObject(const FGameplayTag InTag)
{
	if (SaveTags::IsValidTag(InTag)) return nullptr;
	if (TObjectPtr<UToroSaveObject>* Ptr = SaveObjects.Find(InTag))
	{
		if (*Ptr) return *Ptr;
		SaveObjects.Remove(InTag);
	}

	return nullptr;
}

bool UToroSaveManager::CreateSaveObject(const TSubclassOf<UToroSaveObject> Class, const FGameplayTag InTag)
{
	if (!Class || !SaveTags::IsValidTag(InTag) || FindSaveObject(InTag)) return false;
	UToroSaveObject* Obj = UToroSaveObject::Create(this, Class, InTag);
	if (Obj)
	{
		SaveObjects.Add(InTag, Obj);
		Obj->OnSaveActivity.AddUObject(this, &UToroSaveManager::OnSaveActivity);
	}
	return IsValid(Obj);
}

void UToroSaveManager::OnSaveActivity(const UToroSaveObject* Save, const ESaveGameActivity Type) const
{
	OnSaveIO.Broadcast(Save, Type);
}

void UToroSaveManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	const UToroUtilSettings* Settings = UToroUtilSettings::Get();
	for (const TPair<FGameplayTag, TSoftClassPtr<UToroSaveObject>>& Save : Settings->DefaultSaves)
	{
		if (Save.Value.LoadSynchronous()) CreateSaveObject(Save.Value.LoadSynchronous(), Save.Key);
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
