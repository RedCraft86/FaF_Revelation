// Copyright (C) RedCraft86. All Rights Reserved.

#include "SaveSystem/ToroSaveSystem.h"
#include "ToroRuntimeSettings.h"

UToroSaveSystem::UToroSaveSystem()
{
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
