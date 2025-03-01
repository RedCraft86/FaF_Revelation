﻿// Copyright (C) RedCraft86. All Rights Reserved.

#include "Characters/ToroPlayerBase.h"
#include "Components/CapsuleComponent.h"
#include "Framework/ToroPlayerController.h"

UE_DEFINE_GAMEPLAY_TAG(Tag_Player, "Characters.Player");

AToroPlayerBase::AToroPlayerBase() : SlowTickInterval(0.1f), ControlFlags(DEFAULT_PLAYER_CONTROL_FLAGS)
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	AutoReceiveInput = EAutoReceiveInput::Player0;

	FootstepAudio = CreateDefaultSubobject<UAudioComponent>("FootstepAudio");
	FootstepAudio->SetRelativeLocation(FVector(0.0f, 0.0f, -60.0f));
	FootstepAudio->SetupAttachment(GetCapsuleComponent());

	PlayerLight = CreateDefaultSubobject<UPointLightComponent>("PlayerLight");
	PlayerLight->SetupAttachment(GetCapsuleComponent());
	
	CharacterID = Tag_Player;
	LightSettings.Intensity = 0.025f;
	LightSettings.AttenuationRadius = 500.0f;
	LightSettings.bUseTemperature = true;
	LightSettings.Temperature = 12000.0f;
	LightSettings.bLightCastShadows = false;
	LightSettings.bUseInverseSquaredFalloff = true;
	LightSettings.LightFalloffExponent = 1.0f;
	LightSettings.SpecularScale = 0.0f;
	ULightingDataLibrary::SetPointLightProperties(PlayerLight, LightSettings);
}

EToroValidPins AToroPlayerBase::GetToroPlayerCharacter(AToroPlayerBase*& OutObject,
	const UObject* WorldContextObject, const TSubclassOf<AToroPlayerBase>& Class, const int32 PlayerIndex)
{
	AToroPlayerBase* Obj = Cast<AToroPlayerBase>(UGameplayStatics::GetPlayerCharacter(WorldContextObject, PlayerIndex));
	OutObject = IsValid(Obj) && (!Class || Obj->IsA(Class)) ? Obj : nullptr;
	return IsValid(OutObject) ? EToroValidPins::Valid : EToroValidPins::NotValid;
}

void AToroPlayerBase::TeleportPlayer(const FVector& InLocation, const FRotator& InRotation)
{
	SetActorLocation(InLocation);
	Controller->SetControlRotation(InRotation);
}

void AToroPlayerBase::OverrideControlFlags(const int32 InFlags)
{
	for (const EPlayerControlFlags Enum : TEnumRange<EPlayerControlFlags>())
	{
		if (InFlags & Enum)
		{
			SetControlFlag(Enum);
		}
		else
		{
			UnsetControlFlag(Enum);
		}
	}
}

void AToroPlayerBase::SetControlFlag(const EPlayerControlFlags InFlag)
{
	if (InFlag == PCF_None) return;
	if (!HasControlFlag(InFlag))
	{
		ControlFlags |= InFlag;
	}
}

void AToroPlayerBase::UnsetControlFlag(const EPlayerControlFlags InFlag)
{
	if (InFlag == PCF_None) return;
	if (HasControlFlag(InFlag))
	{
		ControlFlags &= ~InFlag;
	}
}

bool AToroPlayerBase::HasControlFlag(const EPlayerControlFlags InFlag) const
{
	if (InFlag == PCF_None) return false;
	return ControlFlags & InFlag;
}

void AToroPlayerBase::SetStateFlag(const EPlayerStateFlags InFlag)
{
	if (InFlag == PSF_None) return;
	StateFlags |= InFlag;
}

void AToroPlayerBase::UnsetStateFlag(const EPlayerStateFlags InFlag)
{
	if (InFlag == PSF_None) return;
	StateFlags &= ~InFlag;
}

bool AToroPlayerBase::HasStateFlag(const EPlayerStateFlags InFlag) const
{
	if (InFlag == PSF_None) return false;
	return StateFlags & InFlag;
}

void AToroPlayerBase::AddLockFlag(const FPlayerLockFlag& InFlag)
{
	LockFlags.Remove(NAME_None);
	if (InFlag.IsValidFlag()) LockFlags.Add(*InFlag);
}

void AToroPlayerBase::ClearLockFlag(const FPlayerLockFlag& InFlag)
{
	LockFlags.Remove(NAME_None);
	if (InFlag.IsValidFlag()) LockFlags.Remove(*InFlag);
}

bool AToroPlayerBase::HasLockFlag(const FPlayerLockFlag& InFlag) const
{
	return InFlag.IsValidFlag() && LockFlags.Contains(*InFlag);
}

void AToroPlayerBase::ExitCinematic()
{
	if (CinematicActor)
	{
		CinematicActor = nullptr;
		ClearLockFlag(Tag_LockCinematic.GetTag());
		PlayerController->SetCinematicMode(false, false,
			false, true, true);
	}
}

void AToroPlayerBase::EnterCinematic(AActor* InActor)
{
	if (!CinematicActor)
	{
		CinematicActor = InActor;
		AddLockFlag(Tag_LockCinematic.GetTag());
		PlayerController->SetCinematicMode(true, true,
			false, true, true);
	}
}

void AToroPlayerBase::SetLightSettings(const FPointLightProperties& InSettings)
{
	LightSettings = InSettings;
	ULightingDataLibrary::SetPointLightProperties(PlayerLight, LightSettings);
}

void AToroPlayerBase::BeginPlay()
{
	Super::BeginPlay();
	GameMode = AToroGameMode::Get(this);
	PlayerController = AToroPlayerController::Get(this);
	GameInstance = UToroGameInstance::Get(this);
}

void AToroPlayerBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ULightingDataLibrary::SetPointLightProperties(PlayerLight, LightSettings);
}
