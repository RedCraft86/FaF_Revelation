// Copyright (C) RedCraft86. All Rights Reserved.

#include "SaveSystem/ToroGlobalSave.h"
#include "Helpers/WindowsHelpers.h"
#include "ToroUtilities.h"

namespace ContentTags
{
	DEFINE_GAMEPLAY_TAG(Content)
	DEFINE_GAMEPLAY_TAG_CHILD(Content, Backgrounds)
	DEFINE_GAMEPLAY_TAG_CHILD(Content, Character)
	DEFINE_GAMEPLAY_TAG_CHILD(Content, Ending)
	DEFINE_GAMEPLAY_TAG_CHILD(Content, Mode)
	DEFINE_GAMEPLAY_TAG_CHILD(Content, Misc)
}

void UToroGlobalSave::DeleteData()
{
#if !WITH_EDITOR
	WindowsHelpers::OpenDialogue(TEXT("Report this to the dev!"),
		TEXT("Attempting to delete global persistent data, which should not be possible."),
		EWindowsDialogueType::Ok, EWindowsDialogueIcon::Error);
#else
	UE_LOG(LogToroUtilities, Error, TEXT("Cannot delete global persistent data!"));
#endif
}

void UToroGlobalSave::SerializeData(FArchive& Ar)
{
	Ar << Content;
	Ar << Cutscenes;
}
