// Copyright (C) RedCraft86. All Rights Reserved.

#include "PlayerTypes.h"

void FPlayerStamina::TimedTick(const int32 StateFlags)
{
	const bool bDraining = StateFlags & PSF_Run;
	Delta = IsInReserve()
		? (bDraining ? -ReserveRates.X : ReserveRates.Y)
		: (bDraining ? -NormalRates.X : NormalRates.Y);

	Stamina = FMath::Clamp(Stamina + Delta, 0.0f, GetMaxStamina());
}

USoundBase* FSurfaceFootstep::GetSurfaceSound(const EPhysicalSurface Surface) const
{
	const TObjectPtr<USoundBase>* Sound = Sounds.Find(Surface);
	return Sound ? *Sound : nullptr;
}

#if WITH_EDITOR
void FSurfaceFootstep::FillSlots()
{
	if (const UEnum* Enum = StaticEnum<EPhysicalSurface>())
	{
		for (int32 i = 0; i < Enum->NumEnums(); i++)
		{
			const EPhysicalSurface Type = static_cast<EPhysicalSurface>(i);
			if (Type == SurfaceType_Default || Enum->HasMetaData(TEXT("Hidden"), i))
			{
				Sounds.Remove(TEnumAsByte(Type));
			}
			else
			{
				Sounds.Add(TEnumAsByte(Type));
			}
		}
	}
}
#endif

float FPlayerFootsteps::GetFootstepInterval(const int32 StateFlags) const
{
	if (StateFlags & PSF_Run) return RunSounds.Interval;
	if (StateFlags & PSF_Crouch) return SneakSounds.Interval;
	return WalkSounds.Interval;
}

USoundBase* FPlayerFootsteps::GetFootstepSound(const int32 StateFlags, const EPhysicalSurface Surface) const
{
	USoundBase* Sound;
	if (StateFlags & PSF_Run)
	{
		Sound = RunSounds.GetSurfaceSound(Surface);
	}
	else if (StateFlags & PSF_Crouch)
	{
		Sound = SneakSounds.GetSurfaceSound(Surface);
	}
	else
	{
		Sound = WalkSounds.GetSurfaceSound(Surface);
	}
	return Sound ? Sound : DefaultSound.Get();
}

#if WITH_EDITOR
void FPlayerFootsteps::FillSlots()
{
	WalkSounds.FillSlots();
	RunSounds.FillSlots();
	SneakSounds.FillSlots();
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
