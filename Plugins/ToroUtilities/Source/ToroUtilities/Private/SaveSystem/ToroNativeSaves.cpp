// Copyright (C) RedCraft86. All Rights Reserved.

#include "SaveSystem/ToroNativeSaves.h"

#include "ToroSettings.h"
#include "ToroUtilities.h"

namespace SaveTags
{
	DEFINE_GAMEPLAY_TAG_CHILD(Save, GlobalSave)
	DEFINE_GAMEPLAY_TAG_CHILD(Save, GameSave)
}

void UToroGlobalSave::DeleteData()
{
	UE_LOG(LogToroUtilities, Warning, TEXT("Cannot delete global persistent data!"));
}

void UToroGlobalSave::SerializeData(FArchive& Ar)
{
	Ar << Content;
	Ar << Achievements;
}
