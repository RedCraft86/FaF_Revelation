// Copyright (C) RedCraft86. All Rights Reserved.

#include "SaveSystem/ToroNativeSaves.h"
#include "Helpers/WindowsHelpers.h"
#include "ToroRuntime.h"

namespace ContentTags
{
	DEFINE_GAMEPLAY_TAG(Content)
	DEFINE_GAMEPLAY_TAG_CHILD(Content, Cutscene)
	DEFINE_GAMEPLAY_TAG_CHILD(Content, Tutorial)
	DEFINE_GAMEPLAY_TAG_CHILD(Content, Character)
	DEFINE_GAMEPLAY_TAG_CHILD(Content, Ending)
	DEFINE_GAMEPLAY_TAG_CHILD(Content, Misc)
}

UToroGlobalSave::UToroGlobalSave()
{
	bSingleSlot = true;
}

void UToroGlobalSave::DeleteData()
{
#if !WITH_EDITOR
	WindowsHelpers::OpenDialogue(TEXT("Report this to the dev!"),
		TEXT("Attempting to delete global persistent data, which should not be possible."),
		EWindowsDialogueType::Ok, EWindowsDialogueIcon::Error);
#else
	UE_LOG(LogToroRuntime, Error, TEXT("Cannot delete global persistent data!"));
#endif
}

void UToroGlobalSave::SerializeData(FArchive& Ar)
{
	Ar << Content;
	Ar << Achievements;
}

UToroGameSave::UToroGameSave(): PlayTime(0.0f)
{
	bSingleSlot = false;
}

void UToroGameSave::DeleteData()
{
	Super::DeleteData();
	PlayTime = 0.0f;
}

void UToroGameSave::SerializeData(FArchive& Ar)
{
	Ar << PlayTime;
}
