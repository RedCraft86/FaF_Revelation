// Copyright (C) RedCraft86. All Rights Reserved.

#include "SaveSystem/ToroGameSave.h"

UToroGameSave::UToroGameSave(): PlayTime(0.0f)
{
	bSingleSlot = false;
}

void UToroGameSave::DeleteData()
{
	Super::DeleteData();
	PlayTime = 0.0f;
	Archives.Empty();
	Inventory.Empty();
}

void UToroGameSave::SerializeData(FArchive& Ar)
{
	Ar << PlayTime;
	Ar << Archives;
	Ar << Inventory;
}
