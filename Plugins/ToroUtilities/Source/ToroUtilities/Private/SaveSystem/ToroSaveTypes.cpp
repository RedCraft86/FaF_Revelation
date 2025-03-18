// Copyright (C) RedCraft86. All Rights Reserved.

#include "SaveSystem/ToroSaveTypes.h"
#include "ToroUtilities.h"

void UToroGlobalSave::DeleteData()
{
	UE_LOG(LogToroUtilities, Warning, TEXT("Cannot delete global persistent data!"));
}

void UToroGlobalSave::SerializeData(FArchive& Ar)
{
	Ar << Endings;
	Ar << Achievements;
	Ar << Content;
	Ar << Sections;
}

void UToroGameSave::DeleteData()
{
	Super::DeleteData();
	PlayTime = 0;
}

void UToroGameSave::SaveObject(const TFunction<void(const ESaveGameError)>& Callback)
{
	const FDateTime Now = FDateTime::Now();
	PlayTime += (Now - CachedTime).GetTicks();
	CachedTime = Now;

	Super::SaveObject(Callback);
}
