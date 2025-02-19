// Copyright (C) RedCraft86. All Rights Reserved.

#include "SaveSystem/BaseSaveObjects.h"
#include "ToroRuntime.h"

UE_DEFINE_GAMEPLAY_TAG(Tag_GlobalSave, "Saves.Global");
UE_DEFINE_GAMEPLAY_TAG(Tag_GameSave, "Saves.Game");

void UGlobalSaveObjectBase::DeleteData()
{
	UE_LOG(LogToroRuntime, Warning, TEXT("Cannot delete global persistent data!"));
}

void UGlobalSaveObjectBase::SerializeData(FArchive& Ar)
{
	Ar << Endings;
	Ar << Content;
	Ar << Sections;
}

void UGameSaveObjectBase::DeleteData()
{
	Super::DeleteData();
	PlayTime = 0.0f;
	Sequence.Empty();
	Inventory.Empty();
}

void UGameSaveObjectBase::OnCreation()
{
	CachedTime = FDateTime::Now();
}

void UGameSaveObjectBase::SaveObject(const TFunction<void(const ESaveGameError)>& Callback)
{
	const FDateTime Now = FDateTime::Now();
	PlayTime += (Now - CachedTime).GetTicks();
	CachedTime = Now;

	Super::SaveObject(Callback);
}

void UGameSaveObjectBase::SerializeData(FArchive& Ar)
{
	Ar << PlayTime;
	Ar << Sequence;
	Ar << Inventory;
}
