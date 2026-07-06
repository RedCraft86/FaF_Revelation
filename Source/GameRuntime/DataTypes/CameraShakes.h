// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "Shakes/LegacyCameraShake.h"
#include "CameraShakes.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class UCamShake_Walk final : public ULegacyCameraShake
{
	GENERATED_BODY()

public:

	UCamShake_Walk()
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
};

UCLASS(NotBlueprintable, BlueprintType)
class UCamShake_Run final : public ULegacyCameraShake
{
	GENERATED_BODY()

public:

	UCamShake_Run()
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
};
