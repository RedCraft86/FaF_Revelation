// Copyright (C) RedCraft86. All Rights Reserved.

#include "PlayerTypes.h"

void FPlayerStamina::TimedTick(const int32 StateFlags, const bool bIsMoving)
{
	const bool bDraining = bIsMoving && StateFlags & PSF_Run;
	Delta = IsInReserve()
		? (bDraining ? -ReserveRates.X : ReserveRates.Y)
		: (bDraining ? -NormalRates.X : NormalRates.Y);

	Stamina = FMath::Clamp(Stamina + Delta, 0.0f, GetMaxStamina());
}

float FPlayerFootsteps::GetFootstepInterval(const int32 StateFlags) const
{
	if (StateFlags & PSF_Run) return Intervals.Y;
	if (StateFlags & PSF_Crouch) return Intervals.Z;
	return Intervals.X;
}

USoundBase* FPlayerFootsteps::GetFootstepSound(const EPhysicalSurface Surface) const
{
	const TObjectPtr<USoundBase> Sound = SurfaceSounds.FindRef(Surface);
	return Sound ? Sound : DefaultSound;
}

#if WITH_EDITOR
void FPlayerFootsteps::FillSlots()
{
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
}
#endif

UCamShake_Walk::UCamShake_Walk()
{
	bSingleInstance = true;
	OscillationDuration = 0.2f;

	RotOscillation.Pitch.Amplitude = 0.3f;
	RotOscillation.Pitch.Frequency = 12.0f;
	RotOscillation.Pitch.InitialOffset = EOO_OffsetZero;

	RotOscillation.Yaw.Amplitude = 0.3f;
	RotOscillation.Yaw.Frequency = 6.0f;
	RotOscillation.Yaw.InitialOffset = EOO_OffsetZero;

	RotOscillation.Roll.Amplitude = 0.2f;
	RotOscillation.Roll.Frequency = 3.0f;
	RotOscillation.Roll.InitialOffset = EOO_OffsetZero;

	LocOscillation.Z.Amplitude = 0.2f;
	LocOscillation.Z.Frequency = 12.0f;
}

UCamShake_Run::UCamShake_Run()
{
	bSingleInstance = true;
	OscillationDuration = 0.2f;

	RotOscillation.Pitch.Amplitude = 0.75f;
	RotOscillation.Pitch.Frequency = 17.0f;
	RotOscillation.Pitch.InitialOffset = EOO_OffsetZero;

	RotOscillation.Yaw.Amplitude = 0.5f;
	RotOscillation.Yaw.Frequency = 8.5f;
	RotOscillation.Yaw.InitialOffset = EOO_OffsetZero;

	RotOscillation.Roll.Amplitude = 0.2f;
	RotOscillation.Roll.Frequency = 4.75f;
	RotOscillation.Roll.InitialOffset = EOO_OffsetZero;

	LocOscillation.Z.Amplitude = 2.5f;
	LocOscillation.Z.Frequency = 17.0f;
}
