// Copyright (C) RedCraft86. All Rights Reserved.

#include "DataTypes/FootstepTypes.h"

#if WITH_EDITOR
int32 UFootstepDatabase::ValidEntries() const
{
	int32 Count = 0;
	if (const UEnum* Enum = StaticEnum<EPhysicalSurface>())
	{
		for (int32 i = 1; i < Enum->NumEnums(); i++)
		{
			const EPhysicalSurface Type = static_cast<EPhysicalSurface>(i);
			if (Walking.Contains(Type) && Walking.FindRef(Type))
			{
				Count += 1;
			}
			if (Running.Contains(Type) && Running.FindRef(Type))
			{
				Count += 1;
			}
			if (Crouching.Contains(Type) && Crouching.FindRef(Type))
			{
				Count += 1;
			}
		}
	}
	return Count;
}

void UFootstepDatabase::UpdateEntries()
{
	if (FApp::IsGame()) return;
	if (const UEnum* Enum = StaticEnum<EPhysicalSurface>())
	{
		for(int32 i = 0; i < Enum->NumEnums(); i++)
		{
			const EPhysicalSurface Type = static_cast<EPhysicalSurface>(i);
			if (Enum->HasMetaData(TEXT("Hidden"), i))
			{
				Walking.Remove(TEnumAsByte(Type));
				Running.Remove(TEnumAsByte(Type));
				Crouching.Remove(TEnumAsByte(Type));
			}
			else 
			{
				if (!Walking.Contains(TEnumAsByte(Type))) Walking.Add(TEnumAsByte(Type));
				if (!Running.Contains(TEnumAsByte(Type))) Running.Add(TEnumAsByte(Type));
				if (!Crouching.Contains(TEnumAsByte(Type))) Crouching.Add(TEnumAsByte(Type));
			}
		}
	}
}

void UFootstepDatabase::PostLoad()
{
	Super::PostLoad();
	UpdateEntries();
}

void UFootstepDatabase::PostInitProperties()
{
	Super::PostInitProperties();
	UpdateEntries();
}

void UFootstepDatabase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	UpdateEntries();
}
#endif