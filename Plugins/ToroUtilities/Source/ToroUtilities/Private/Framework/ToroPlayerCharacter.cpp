// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroPlayerCharacter.h"
#include "Components/PointLightComponent.h"
#include "Components/CapsuleComponent.h"
#include "Framework/ToroGameInstance.h"
#include "Framework/ToroGameMode.h"
#include "ToroUtilities.h"

AToroPlayerCharacter::AToroPlayerCharacter(): SlowTickInterval(0.1f), LockTags({PlayerLockTag::TAG_Startup.GetTag()})
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
	ULightingDataLibrary::SetPointLightProperties(PlayerLight, LightSettings);
}

void AToroPlayerCharacter::AddLockTag(const FGameplayTag InTag)
{
	if (PlayerLockTag::IsValidTag(InTag)) LockTags.AddTag(InTag);
}

void AToroPlayerCharacter::ClearLockTag(const FGameplayTag InTag)
{
	if (PlayerLockTag::IsValidTag(InTag)) LockTags.RemoveTag(InTag);
}

bool AToroPlayerCharacter::HasLockTag(const FGameplayTag InTag) const
{
	return LockTags.HasTagExact(InTag);
}

void AToroPlayerCharacter::SetLightSettings(const FPointLightProperties& InSettings)
{
	LightSettings = InSettings;
	ULightingDataLibrary::SetPointLightProperties(PlayerLight, LightSettings);
}

void AToroPlayerCharacter::EnterCinematic(AActor* CinematicActor)
{
	if (AToroPlayerController* PC = GetPlayerController()) PC->EnterCinematic(CinematicActor);
}

void AToroPlayerCharacter::ExitCinematic()
{
	if (AToroPlayerController* PC = GetPlayerController()) PC->ExitCinematic();
}

void AToroPlayerCharacter::FadeToBlack(const float InTime, const bool bAudio) const
{
	if (const AToroPlayerController* PC = GetPlayerController())
	{
		if (FMath::IsNearlyZero(InTime))
		{
			PC->PlayerCameraManager->SetManualCameraFade(1.0f, FLinearColor::Black, bAudio);
		}
		else
		{
			PC->PlayerCameraManager->StartCameraFade(0.0f, 1.0f,
				InTime, FLinearColor::Black, bAudio, true);
		}
	}
}

void AToroPlayerCharacter::FadeFromBlack(const float InTime, const bool bAudio) const
{
	if (const AToroPlayerController* PC = GetPlayerController())
	{
		if (FMath::IsNearlyZero(InTime))
		{
			PC->PlayerCameraManager->SetManualCameraFade(0.0f, FLinearColor::Black, bAudio);
		}
		else
		{
			PC->PlayerCameraManager->StartCameraFade(1.0f, 0.0f,
				InTime, FLinearColor::Black, bAudio, true);
		}
	}
}

void AToroPlayerCharacter::ClearFade() const
{
	if (const AToroPlayerController* PC = GetPlayerController())
	{
		PC->PlayerCameraManager->StopCameraFade();
	}
}

void AToroPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (UToroUtilSettings::Get()->IsOnLaunchMap(this))
	{
		LockTags.AddTag(PlayerLockTag::TAG_MainMenu);
	}

	GameMode = AToroGameMode::Get(this);
	GameInstance = GetGameInstance<UToroGameInstance>();
	GetWorldTimerManager().SetTimer(SlowTickTimer, this,
		&AToroPlayerCharacter::SlowTick, SlowTickInterval, true, 0.0f);
}

void AToroPlayerCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	LockTags.RemoveTag(PlayerLockTag::TAG_PlayerLock);
	ULightingDataLibrary::SetPointLightProperties(PlayerLight, LightSettings);
}
