// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroPlayerCharacter.h"
#include "Framework/ToroPlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Libraries/ToroLightingUtils.h"

namespace PlayerLockTags
{
	DEFINE_GAMEPLAY_TAG(PlayerLock)
	DEFINE_GAMEPLAY_TAG_CHILD(PlayerLock, MainMenu)
	DEFINE_GAMEPLAY_TAG_CHILD(PlayerLock, Startup)
	DEFINE_GAMEPLAY_TAG_CHILD(PlayerLock, Loading)
	DEFINE_GAMEPLAY_TAG_CHILD(PlayerLock, Dialogue)
	DEFINE_GAMEPLAY_TAG_CHILD(PlayerLock, Cinematic)
	DEFINE_GAMEPLAY_TAG_CHILD(PlayerLock, GameOver)
}

AToroPlayerCharacter::AToroPlayerCharacter(): SlowTickInterval(0.1f), LockTags({PlayerLockTags::TAG_Startup.GetTag()})
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	AutoReceiveInput = EAutoReceiveInput::Player0;
	
	PlayerLight = CreateDefaultSubobject<UPointLightComponent>("PlayerLight");
	PlayerLight->SetupAttachment(GetCapsuleComponent());

	CharacterID = CharacterTags::TAG_Player;
	LightSettings.Intensity = 0.025f;
	LightSettings.AttenuationRadius = 500.0f;
	LightSettings.bUseTemperature = true;
	LightSettings.Temperature = 12000.0f;
	LightSettings.bLightCastShadows = false;
	LightSettings.bUseInverseSquaredFalloff = true;
	LightSettings.LightFalloffExponent = 1.0f;
	LightSettings.SpecularScale = 0.0f;
	UToroLightingUtils::SetPointLightProperties(PlayerLight, LightSettings);
}

void AToroPlayerCharacter::AddLockTag(const FGameplayTag InTag)
{
	if (PlayerLockTags::IsValidTag(InTag)) LockTags.AddTag(InTag);
}

void AToroPlayerCharacter::ClearLockTag(const FGameplayTag InTag)
{
	if (PlayerLockTags::IsValidTag(InTag)) LockTags.RemoveTag(InTag);
}

bool AToroPlayerCharacter::HasLockTag(const FGameplayTag InTag) const
{
	return LockTags.HasTagExact(InTag);
}

void AToroPlayerCharacter::SetLightSettings(const FPointLightProperties& InSettings)
{
	LightSettings = InSettings;
	UToroLightingUtils::SetPointLightProperties(PlayerLight, LightSettings);
}

void AToroPlayerCharacter::EnterCinematic(AActor* CinematicActor)
{
	if (AToroPlayerController* PC = GetController<AToroPlayerController>())
	{
		PC->EnterCinematic(CinematicActor);
		AddLockTag(PlayerLockTags::TAG_Cinematic);
	}
}

void AToroPlayerCharacter::ExitCinematic()
{
	if (AToroPlayerController* PC = GetController<AToroPlayerController>())
	{
		PC->ExitCinematic();
		ClearLockTag(PlayerLockTags::TAG_Cinematic);
	}
}

void AToroPlayerCharacter::FadeToBlack(const float InTime, const bool bAudio) const
{
	const AToroPlayerController* PC = GetController<AToroPlayerController>();
	if (APlayerCameraManager* CamManager = PC ? PC->PlayerCameraManager : nullptr)
	{
		if (FMath::IsNearlyZero(InTime))
		{
			CamManager->SetManualCameraFade(1.0f, FLinearColor::Black, bAudio);
		}
		else
		{
			CamManager->StartCameraFade(0.0f, 1.0f,
				InTime, FLinearColor::Black, bAudio, true);
		}
	}
}

void AToroPlayerCharacter::FadeFromBlack(const float InTime, const bool bAudio) const
{
	const AToroPlayerController* PC = GetController<AToroPlayerController>();
	if (APlayerCameraManager* CamManager = PC ? PC->PlayerCameraManager : nullptr)
	{
		if (FMath::IsNearlyZero(InTime))
		{
			CamManager->SetManualCameraFade(0.0f, FLinearColor::Black, bAudio);
		}
		else
		{
			CamManager->StartCameraFade(1.0f, 0.0f,
				InTime, FLinearColor::Black, bAudio, true);
		}
	}
}

void AToroPlayerCharacter::ClearFade() const
{
	const AToroPlayerController* PC = GetController<AToroPlayerController>();
	if (APlayerCameraManager* CamManager = PC ? PC->PlayerCameraManager : nullptr)
	{
		CamManager->StopCameraFade();
	}
}

void AToroPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	// if (UToroFrameworkSettings::Get()->IsOnLaunchMap(this))
	// {
	// 	LockTags.AddTag(PlayerLockTags::TAG_MainMenu);
	// }

	GetWorldTimerManager().SetTimer(SlowTickTimer, this,
		&AToroPlayerCharacter::SlowTick, SlowTickInterval, true, 0.0f);
}

void AToroPlayerCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	LockTags.RemoveTag(PlayerLockTags::TAG_PlayerLock);
	UToroLightingUtils::SetPointLightProperties(PlayerLight, LightSettings);
}
