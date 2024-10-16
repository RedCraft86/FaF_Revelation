﻿// Copyright (C) RedCraft86. All Rights Reserved.

#include "FRCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "FRPlayer.h"
#include "Kismet/GameplayStatics.h"

AFRCharacter::AFRCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	FootstepAudio = CreateDefaultSubobject<UAudioComponent>("FootstepAudio");
	FootstepAudio->SetRelativeLocation({0.0f, 0.0f, -50.0f});
	FootstepAudio->SetupAttachment(GetCapsuleComponent());
	FootstepAudio->bCanPlayMultipleInstances = true;
	FootstepAudio->bAutoActivate = false;
	FootstepAudio->bOverrideAttenuation = true;
	FootstepAudio->AttenuationOverrides.bAttenuate = true;
	FootstepAudio->AttenuationOverrides.AttenuationShapeExtents = FVector::ZeroVector;
	FootstepAudio->AttenuationOverrides.FalloffDistance = 4500.0f;

	FootstepAudio->AttenuationOverrides.bEnableOcclusion = true;
	FootstepAudio->AttenuationOverrides.OcclusionLowPassFilterFrequency = MAX_FILTER_FREQUENCY * 0.5f;
	FootstepAudio->AttenuationOverrides.OcclusionVolumeAttenuation = 0.5f;

	AudioVolumeMulti = 1.0f;
	AudioVolumeCurve.GetRichCurve()->UpdateOrAddKey(500.0f, 1.0f);
	AudioVolumeCurve.GetRichCurve()->UpdateOrAddKey(2500.0f, 0.0f);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->RotationRate = {0.0f, 270.0f, 0.0f};
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void AFRCharacter::PlaySmartAudio(const FName AudioKey)
{
	if (UAudioComponent* Audio = GetKeyedAudio().FindRef(AudioKey))
	{
		if (const AFRPlayerBase* Player = PlayerChar.LoadSynchronous())
		{
			FHitResult Hit;
			FCollisionQueryParams QueryParams; QueryParams.AddIgnoredActor(this); QueryParams.AddIgnoredActor(Player);
			GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation() + FVector(0.0f, 0.0f, 30.0f),
				Player->PlayerCamera->GetComponentLocation(), ECC_Visibility, QueryParams);

			float Dist = -1.0f;
			if (Hit.IsValidBlockingHit())
			{
				const UNavigationPath* Path = UNavigationSystemV1::FindPathToLocationSynchronously(this,
					GetActorLocation(), Player->PlayerCamera->GetComponentLocation(), this);
				
				Dist = Path ? Path->GetPathLength() : 0.0f;
				Audio->SetVolumeMultiplier(AudioVolumeMulti * AudioVolumeCurve.GetValue(FMath::Clamp(Dist, 500.0f, 2500.0f)));
			}
			else
			{
				Audio->SetVolumeMultiplier(AudioVolumeMulti);
			}
			
			Audio->Play();
			if (Dist < 300.0f && AudioKey == FName("Footstep"))
			{
				UGameplayStatics::PlayWorldCameraShake(this, FootstepShake, GetActorLocation(), 200, 400);
			}
			
			OnAudioPlayed.Broadcast(this, Audio, AudioKey, Dist);
		}
	}
}

void AFRCharacter::DisableAI()
{
	if (GetLogicComponent()) GetLogicComponent()->Stop();
	GetCharacterMovement()->StopMovementImmediately();
}

TMap<FName, UAudioComponent*> AFRCharacter::GetKeyedAudio_Implementation() const
{
	return {{"Footstep", FootstepAudio}};
}

USceneComponent* AFRCharacter::GetLookAtComponent_Implementation() const
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
