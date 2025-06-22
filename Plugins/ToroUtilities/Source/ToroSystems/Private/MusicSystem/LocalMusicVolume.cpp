// Copyright (C) RedCraft86. All Rights Reserved.

#include "MusicSystem/LocalMusicVolume.h"
#include "MusicSystem/GameMusicManager.h"
#include "Framework/ToroPlayerCharacter.h"
#include "Components/AudioComponent.h"

ALocalMusicVolume::ALocalMusicVolume(): Cooldown(5.0f), Volume(1.0f), StartRange(0.0f)
	, FadeInOut(1.0f), AllowedStates({0}), bInVolume(false), bOnCooldown(false), MusicState(0)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SoundComponent = CreateDefaultSubobject<UAudioComponent>("SoundComponent");
	SoundComponent->SetupAttachment(GetRootComponent());
	SoundComponent->SetUISound(true);
	SoundComponent->bCanPlayMultipleInstances = false;
	SoundComponent->bAutoActivate = false;
	SoundComponent->bIsMusic = true;

#if WITH_EDITOR
	BrushColor = FColor(180, 0, 255);
#endif
}

void ALocalMusicVolume::UpdateAudio()
{
	if (IsEnabled() && bInVolume && !bOnCooldown && AllowedStates.Contains(MusicState))
	{
		SoundComponent->FadeIn(FadeInOut.X, 1.0f,
			FMath::FRandRange(StartRange.X, StartRange.Y));

		if (bDipMainTheme && Manager)
		{
			Manager->AddDipRequest(this);
		}

		if (Cooldown > 0.01f)
		{
			bOnCooldown = true;
			FTimerHandle Handle;
			GetWorldTimerManager().SetTimer(Handle, this,
				&ALocalMusicVolume::CooldownFinish, Cooldown, false);
		}
	}
	else
	{
		SoundComponent->FadeOut(FadeInOut.Y, 0.0f);
		if (Manager)
		{
			Manager->RemoveDipRequest(this);
		}
	}
}

void ALocalMusicVolume::BeginPlay()
{
	Super::BeginPlay();
	SoundComponent->SetSound(Sound);
	SoundComponent->SetVolumeMultiplier(Volume);
	GetWorldTimerManager().SetTimerForNextTick([this]()
	{
		Manager = UGameMusicManager::Get(this);
		Manager->OnThemeChanged.AddUObject(this, &ALocalMusicVolume::StopUntilNextTrigger);
		Manager->OnAudioDipped.AddUObject(this, &ALocalMusicVolume::StopUntilNextTrigger);
		Manager->OnStateChanged.AddUObject(this, &ALocalMusicVolume::OnMusicStateChanged);
	});
}

void ALocalMusicVolume::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (CharacterTags::IsPlayer(OtherActor))
	{
		bInVolume = true;
		UpdateAudio();
	}
}

void ALocalMusicVolume::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	if (CharacterTags::IsPlayer(OtherActor))
	{
		bInVolume = false;
		UpdateAudio();
	}
}
