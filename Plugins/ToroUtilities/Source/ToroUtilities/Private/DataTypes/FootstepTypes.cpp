// Copyright (C) RedCraft86. All Rights Reserved.

#include "FootstepTypes.h"

#if WITH_EDITOR
int32 UFootstepDatabase::GetValidData() const
{
	int32 Count = 0;
	if (const UEnum* Enum = StaticEnum<EPhysicalSurface>())
	{
		for (int32 i = 1; i < Enum->NumEnums(); i++)
		{
			if (Surfaces.FindRef(static_cast<EPhysicalSurface>(i)))
			{
				Count += 1;
			}
		}
	}
	return Count;
}

void UFootstepDatabase::UpdateData()
{
	Super::UpdateData();
	if (const UEnum* Enum = StaticEnum<EPhysicalSurface>())
	{
		for(int32 i = 0; i < Enum->NumEnums(); i++)
		{
			const EPhysicalSurface Type = static_cast<EPhysicalSurface>(i);
			if (Enum->HasMetaData(TEXT("Hidden"), i))
			{
				Surfaces.Remove(TEnumAsByte(Type));
			}
			else if (!Surfaces.Contains(TEnumAsByte(Type)))
			{
				Surfaces.Add(TEnumAsByte(Type));
			}
		}
	}
}
#endif