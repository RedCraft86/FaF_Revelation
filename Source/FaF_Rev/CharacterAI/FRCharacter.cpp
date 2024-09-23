// Copyright (C) RedCraft86. All Rights Reserved.

#include "FRCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "FRPlayer.h"

AFRCharacter::AFRCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	FootstepAudio = CreateDefaultSubobject<UAudioComponent>("FootstepAudio");
	FootstepAudio->SetRelativeLocation({0.0f, 0.0f, -50.0f});
	FootstepAudio->SetupAttachment(GetCapsuleComponent());
	FootstepAudio->bAutoActivate = false;
	FootstepAudio->bOverrideAttenuation = true;
	FootstepAudio->AttenuationOverrides.bAttenuate = true;
	FootstepAudio->AttenuationOverrides.AttenuationShapeExtents = FVector::ZeroVector;
	FootstepAudio->AttenuationOverrides.FalloffDistance = 4500.0f;

	FootstepAudio->AttenuationOverrides.bEnableOcclusion = true;
	FootstepAudio->AttenuationOverrides.OcclusionLowPassFilterFrequency = MAX_FILTER_FREQUENCY * 0.5f;
	FootstepAudio->AttenuationOverrides.OcclusionVolumeAttenuation = 0.5f;

	AudioVolumeCurve.GetRichCurve()->UpdateOrAddKey(500.0f, 1.0f);
	AudioVolumeCurve.GetRichCurve()->UpdateOrAddKey(2500.0f, 0.0f);
}

void AFRCharacter::PlaySmartAudio(UAudioComponent* InComponent)
{
	if (InComponent && InComponent->Sound)
	{
		if (const AFRPlayerBase* Player = PlayerChar.LoadSynchronous())
		{
			const UNavigationPath* Path = UNavigationSystemV1::FindPathToLocationSynchronously(this,
				GetActorLocation(), Player->PlayerCamera->GetComponentLocation(), this);

			const float Dist = FMath::Clamp(Path ? Path->GetPathLength() : 0.0f, 500.0f, 2500.0f);
			InComponent->SetVolumeMultiplier(AudioVolumeCurve.GetValue(Dist));
			InComponent->Play();
			
			OnAudioPlayed.Broadcast(this, InComponent);
		}
	}
}

void AFRCharacter::DisableAI()
{
	if (GetLogicComponent()) GetLogicComponent()->Stop();
	GetCharacterMovement()->StopMovementImmediately();
}

USceneComponent* AFRCharacter::GetLookAtData_Implementation()
{
	if (const AFRPlayerBase* Player = PlayerChar.LoadSynchronous())
	{
		return Player->PlayerCamera;
	}

	return nullptr;
}

void AFRCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (PlayerChar.IsNull()) PlayerChar = AFRPlayerBase::Get(this);
}
