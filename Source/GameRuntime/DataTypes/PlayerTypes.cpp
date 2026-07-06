// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "PlayerTypes.h"

bool FPlayerStaminaInfo::TickStamina(const float DeltaTime, const bool bRunning)
{
	if (!TickInterval.TickCooldown(DeltaTime))
	{
		return false;
	}

	if (bRunning)
	{
		Amount = FMath::Max(0.0f, Amount - DrainRate);
		if (Amount < 0.01f)
		{
			Amount = 0.0f;
			bPunished = true;
		}
	}
	else if (Amount < 1.0f)
	{
		Amount = FMath::Min(1.0f, Amount + GainRate);
		if (Amount > 0.5f)
		{
			bPunished = false;
		}
	}

	return true;
}

void FPlayerFootstepInfo::AllocateSlots()
{
#if WITH_EDITOR
	if (const UEnum* Enum = StaticEnum<EPhysicalSurface>())
	{
		for (int32 i = 0; i < Enum->NumEnums(); i++)
		{
			const EPhysicalSurface Type = static_cast<EPhysicalSurface>(i);
			if (Type == SurfaceType_Default || Enum->HasMetaData(TEXT("Hidden"), i))
			{
				SurfaceSounds.Remove(TEnumAsByte(Type));
			}
			else
			{
				SurfaceSounds.FindOrAdd(TEnumAsByte(Type));
			}
		}
	}
#endif
}

float FPlayerFootstepInfo::GetFootstepInterval(const int32 StateFlags) const
{
	if (StateFlags & PSF_Run)
	{
		return Intervals.Y;
	}

	if (StateFlags & PSF_Crouch)
	{
		return Intervals.Z;
	}

	return Intervals.X;
}

USoundBase* FPlayerFootstepInfo::GetFootstepSound(const EPhysicalSurface Surface) const
{
	const TObjectPtr<USoundBase> Sound = SurfaceSounds.FindRef(Surface);
	return Sound ? Sound : DefaultSound;
}
