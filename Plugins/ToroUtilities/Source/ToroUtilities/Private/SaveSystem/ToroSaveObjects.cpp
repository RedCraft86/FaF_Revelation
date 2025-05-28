// Copyright (C) RedCraft86. All Rights Reserved.

#include "SaveSystem/ToroSaveObjects.h"
#include "ToroUtilities.h"

DEFINE_GAMEPLAY_TAG_CHILD(Save, Global)
DEFINE_GAMEPLAY_TAG_CHILD(Save, Game)

void UToroGlobalSave::DeleteData()
{
	UE_LOG(LogToroUtilities, Warning, TEXT("Cannot delete global persistent data!"));
}

void UToroGlobalSave::SerializeData(FArchive& Ar)
{
	Ar << Content;
	Ar << Achievements;
}

void UToroGameSave::DeleteData()
{
	Super::DeleteData();
	PlayTime = 0;
}

void UToroGameSave::SaveObject(const TFunction<void(const ESaveGameStatus)>& Callback)
{
	if (CachedTime.GetTicks() > 5)
	{
		const FDateTime Now = FDateTime::Now();
		PlayTime += (Now - CachedTime).GetTicks();
		CachedTime = Now;
	}

	Super::SaveObject(Callback);
}

void UToroGameSave::SerializeData(FArchive& Ar)
{
	Ar << PlayTime;
}
