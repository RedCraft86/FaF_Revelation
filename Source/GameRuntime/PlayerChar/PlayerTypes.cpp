// Copyright (C) RedCraft86. All Rights Reserved.

#include "PlayerTypes.h"

UE_DEFINE_GAMEPLAY_TAG_CHILD(MainMenu, PlayerLock);
UE_DEFINE_GAMEPLAY_TAG_CHILD(Startup, PlayerLock);
UE_DEFINE_GAMEPLAY_TAG_CHILD(Loading, PlayerLock);
UE_DEFINE_GAMEPLAY_TAG_CHILD(Jumpscare, PlayerLock);
UE_DEFINE_GAMEPLAY_TAG_CHILD(Inventory, PlayerLock);
UE_DEFINE_GAMEPLAY_TAG_CHILD(Hiding, PlayerLock);
UE_DEFINE_GAMEPLAY_TAG_CHILD(Device, PlayerLock);
UE_DEFINE_GAMEPLAY_TAG_CHILD(Guide, PlayerLock);
UE_DEFINE_GAMEPLAY_TAG_CHILD(QTE, PlayerLock);

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

	RotOscillation.Pitch.Amplitude = 0.5f;
	RotOscillation.Pitch.Frequency = 17.0f;
	RotOscillation.Pitch.InitialOffset = EOO_OffsetZero;

	RotOscillation.Yaw.Amplitude = 0.5f;
	RotOscillation.Yaw.Frequency = 8.5f;
	RotOscillation.Yaw.InitialOffset = EOO_OffsetZero;

	RotOscillation.Roll.Amplitude = 0.2f;
	RotOscillation.Roll.Frequency = 4.75f;
	RotOscillation.Roll.InitialOffset = EOO_OffsetZero;

	LocOscillation.Z.Amplitude = 5.0f;
	LocOscillation.Z.Frequency = 17.0f;
}
