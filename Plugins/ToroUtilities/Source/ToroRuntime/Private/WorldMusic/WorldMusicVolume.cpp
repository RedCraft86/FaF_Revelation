// Copyright (C) RedCraft86. All Rights Reserved.

#include "WorldMusic/WorldMusicVolume.h"
#include "Kismet/GameplayStatics.h"

AWorldMusicVolume::AWorldMusicVolume() : bStopOnExit(false), Cooldown(10.0f)
	, FadeTime(1.0), Volume(1.0), StartRange(0.0f)
{
	PrimaryActorTick.bCanEverTick = false;
#if WITH_EDITOR
	if (DebugIcon) DebugIcon->IconPath = TEXT("/ToroUtilities/Icons/MusicVolume.MusicVolume");
#endif
}

void AWorldMusicVolume::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (GetWorldTimerManager().TimerExists(CooldownTimer)) return;
	if (MusicManager && OtherActor == Player)
	{
		MusicManager->PlayLayer(Sound, FadeTime, Volume, StartRange);
	}
}

void AWorldMusicVolume::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	if (bStopOnExit && MusicManager && OtherActor == Player)
	{
		MusicManager->StopLayer(Sound, FadeTime);
	}
	
	GetWorldTimerManager().SetTimer(CooldownTimer, this,
		&AWorldMusicVolume::EmptyFunction, Cooldown, false);
}

void AWorldMusicVolume::BeginPlay()
{
	Super::BeginPlay();
	Player = UGameplayStatics::GetPlayerPawn(this, 0);
	MusicManager = AWorldMusicManager::Get(this);
}
