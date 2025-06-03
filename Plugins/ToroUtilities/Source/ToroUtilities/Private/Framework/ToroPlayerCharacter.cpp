// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Framework/ToroPlayerController.h"
#include "Framework/ToroGameInstance.h"
#include "Framework/ToroGameMode.h"
#include "ToroSettings.h"
#if WITH_EDITOR
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"
#endif

DEFINE_GAMEPLAY_TAG_CHILD(Character, Player)

AToroPlayerCharacter::AToroPlayerCharacter(): SlowTickInterval(0.1f), LockFlags({LockFlag(Startup)})
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	AutoReceiveInput = EAutoReceiveInput::Player0;
	
	PlayerLight = CreateDefaultSubobject<UPointLightComponent>("PlayerLight");
	PlayerLight->SetupAttachment(GetCapsuleComponent());

	CharacterID = Tag_Character_Player;
	LightSettings.Intensity = 0.025f;
	LightSettings.AttenuationRadius = 500.0f;
	LightSettings.bUseTemperature = true;
	LightSettings.Temperature = 12000.0f;
	LightSettings.bLightCastShadows = false;
	LightSettings.bUseInverseSquaredFalloff = true;
	LightSettings.LightFalloffExponent = 1.0f;
	LightSettings.SpecularScale = 0.0f;
	ULightingDataLibrary::SetPointLightData(PlayerLight, LightSettings);
}

void AToroPlayerCharacter::AddLockFlag(const FPlayerLockFlag& InFlag)
{
	if (InFlag.IsValidFlag()) LockFlags.AddUnique(InFlag);
}

void AToroPlayerCharacter::ClearLockFlag(const FPlayerLockFlag& InFlag)
{
	if (InFlag.IsValidFlag()) LockFlags.Remove(InFlag);
}

bool AToroPlayerCharacter::HasLockFlag(const FPlayerLockFlag& InFlag) const
{
	return InFlag.IsValidFlag() && LockFlags.Contains(InFlag);
}

void AToroPlayerCharacter::SetLightSettings(const FPointLightData& InSettings)
{
	LightSettings = InSettings;
	ULightingDataLibrary::SetPointLightData(PlayerLight, LightSettings);
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
	LockFlags.RemoveAll([](const FPlayerLockFlag& InFlag) { return !InFlag.IsValidFlag(); });
	if (UToroSettings::Get()->IsOnLaunchMap(this))
	{
		AddPlayerLock(MainMenu);
	}

	GameMode = AToroGameMode::Get(this);
	GameInstance = GetGameInstance<UToroGameInstance>();
	GetWorldTimerManager().SetTimer(SlowTickTimer, this,
		&AToroPlayerCharacter::SlowTick, SlowTickInterval, true, 0.0f);
}

void AToroPlayerCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (FPlayerLockFlag::DedupeFlags(LockFlags))
	{
#if WITH_EDITOR
		FNotificationInfo Info(INVTEXT("Cannot contain duplicate Lock Flags"));
		Info.bFireAndForget = true;
		Info.FadeOutDuration = 2.0f;
		Info.ExpireDuration = 2.0f;
		Info.bUseLargeFont = false;
		Info.bUseThrobber = false;
		Info.bUseSuccessFailIcons = true;
		Info.bAllowThrottleWhenFrameRateIsLow = false;
		TSharedPtr<SNotificationItem> Notif = FSlateNotificationManager::Get().AddNotification(Info);
		if (Notif.IsValid()) Notif->SetCompletionState(SNotificationItem::CS_Fail);
#endif
	}
	ULightingDataLibrary::SetPointLightData(PlayerLight, LightSettings);
}
