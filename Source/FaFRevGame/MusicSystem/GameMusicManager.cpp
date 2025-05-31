// Copyright (C) RedCraft86. All Rights Reserved.

#include "GameMusicManager.h"
#include "Components/AudioComponent.h"
#include "FaFRevSettings.h"

AGameMusicManager::AGameMusicManager()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	ThemeComponent = CreateDefaultSubobject<UAudioComponent>("ThemeComponent");
	ThemeComponent->SetupAttachment(SceneRoot);
	ThemeComponent->SetUISound(true);
	ThemeComponent->bCanPlayMultipleInstances = false;
	ThemeComponent->bAutoActivate = false;
	ThemeComponent->bIsMusic = true;
}

void AGameMusicManager::ChangeMainTheme(UMetaSoundSource* InTheme)
{
	if (InTheme && ThemeComponent->Sound != InTheme)
	{
		if (ThemeComponent->bIsPaused)
		{
			ThemeComponent->Stop();
			ThemeComponent->SetPaused(false);
			ThemeComponent->SetSound(InTheme);
			ThemeComponent->FadeIn(1.0f);
		}
		else
		{
			ThemeComponent->FadeOut(1.0f, 0.0f);
			GetWorldTimerManager().ClearTimer(ChangeTimer);
			GetWorldTimerManager().SetTimer(ChangeTimer, [this, InTheme]()
			{
				ThemeComponent->SetSound(InTheme);
				ThemeComponent->FadeIn(1.0f);
			}, 1.0f, false);
		}

		SetThemeState(0);
		SetThemeIntensity(0.0f);
	}
}

void AGameMusicManager::SetThemeMuted(const bool bInMuted)
{
	ThemeComponent->SetPaused(bInMuted);
}

void AGameMusicManager::SetThemeState(const uint8 InState) const
{
	GetSoundParamInterface()->SetIntParameter(TEXT("State"), InState);
}

void AGameMusicManager::SetThemeIntensity(const float InIntensity) const
{
	GetSoundParamInterface()->SetFloatParameter(TEXT("Intensity"), InIntensity);
}

void AGameMusicManager::SetPlayerHiding(const bool bInHiding)
{
	if (bPlayerHide != bInHiding)
	{
		bPlayerHide = bInHiding;
		UpdateDippedState();
	}
}

void AGameMusicManager::SetGamePaused(const bool bInPaused)
{
	if (bGamePaused != bInPaused)
	{
		bGamePaused = bInPaused;
		UpdateDippedState();
	}
}

void AGameMusicManager::UpdateDippedState()
{
	if (const bool bTemp = bPlayerHide || bGamePaused; bDipped != bTemp)
	{
		bDipped = bTemp;
		GetSoundParamInterface()->SetBoolParameter(TEXT("Dipped"), bDipped);
	}
}

IAudioParameterControllerInterface* AGameMusicManager::GetSoundParamInterface() const
{
	return Cast<IAudioParameterControllerInterface>(ThemeComponent);
}

void AGameMusicManager::BeginPlay()
{
	Super::BeginPlay();
	if (UMetaSoundSource* Default = UFaFRevSettings::Get()->DefaultTheme.LoadSynchronous())
	{
		ThemeComponent->SetSound(Default);
		ThemeComponent->FadeIn(1.0f);
	}
}
