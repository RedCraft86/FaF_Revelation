// Copyright (C) RedCraft86. All Rights Reserved.

#include "LocalSoundVolume.h"
#include "GameMusicManager.h"
#include "Components/AudioComponent.h"
#include "Player/GamePlayerChar.h"

ALocalSoundVolume::ALocalSoundVolume(): Cooldown(5.0f), Volume(1.0f), StartRange(0.0f)
	, FadeInOut(1.0f), AllowedMusicStates({0}), bInVolume(false), bOnCooldown(false), MusicState(0)
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

void ALocalSoundVolume::UpdateAudio()
{
	if (bInVolume && bOnCooldown && AllowedMusicStates.Contains(MusicState))
	{
		SoundComponent->FadeIn(FadeInOut.X, 1.0f,
			FMath::FRandRange(StartRange.X, StartRange.Y));

		bOnCooldown = true;
		FTimerHandle Handle;
		GetWorldTimerManager().SetTimer(Handle, this,
			&ALocalSoundVolume::CooldownFinish, Cooldown, false);
	}
	else SoundComponent->FadeOut(FadeInOut.Y, 0.0f);
}

void ALocalSoundVolume::BeginPlay()
{
	Super::BeginPlay();
	SoundComponent->SetSound(Sound);
	SoundComponent->SetVolumeMultiplier(Volume);
	GetWorldTimerManager().SetTimerForNextTick([this]()
	{
		if (AGameMusicManager* MusicManager = AGameMusicManager::Get<AGameMusicManager>(this))
		{
			MusicManager->OnThemeChanged.AddUObject(this, &ALocalSoundVolume::StopUntilNextTrigger);
			MusicManager->OnAudioDipped.AddUObject(this, &ALocalSoundVolume::StopUntilNextTrigger);
			MusicManager->OnStateChanged.AddUObject(this, &ALocalSoundVolume::OnMusicStateChanged);
		}
	});
}

void ALocalSoundVolume::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (OtherActor->IsA<AGamePlayerChar>())
	{
		bInVolume = true;
		UpdateAudio();
	}
}

void ALocalSoundVolume::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	if (OtherActor->IsA<AGamePlayerChar>())
	{
		bInVolume = false;
		UpdateAudio();
	}
}
