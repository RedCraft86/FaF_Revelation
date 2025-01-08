// Copyright (C) RedCraft86. All Rights Reserved.

#include "WorldMusic/WorldMusicVolume.h"
#include "Kismet/GameplayStatics.h"

AWorldMusicVolume::AWorldMusicVolume() : bSingleUse(true), bStopOnExit(false)
	, Cooldown(10.0f), Volume(1.0), FadeTime(1.0), StartRange(0.0f)
{
	PrimaryActorTick.bCanEverTick = false;
#if WITH_EDITOR
	if (DebugIcon)
	{
		DebugIcon->SetWorldScale3D(FVector{0.25f});
		DebugIcon->IconPath = TEXT("/ToroUtilities/Icons/MusicVolume.MusicVolume");
	}
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

	if (!bStopOnExit && bSingleUse)
	{
		SetActorEnableCollision(false);
	}
}

void AWorldMusicVolume::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	if (bStopOnExit && MusicManager && OtherActor == Player)
	{
		MusicManager->StopLayer(Sound, FadeTime);
	}

	if (!bSingleUse)
	{
		if (Cooldown > 1.0f) GetWorldTimerManager().SetTimer(CooldownTimer, this,
		   &AWorldMusicVolume::EmptyFunction, Cooldown, false);
	}
	else
	{
		SetActorEnableCollision(false);
	}
}

void AWorldMusicVolume::BeginPlay()
{
	Super::BeginPlay();
	Player = UGameplayStatics::GetPlayerPawn(this, 0);
	MusicManager = AWorldMusicManager::Get(this);
}
