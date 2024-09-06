// Copyright (C) RedCraft86. All Rights Reserved.

#include "GameMusic/WorldMusicInstance.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

AWorldMusicInstance::AWorldMusicInstance()
{
	PrimaryActorTick.bCanEverTick = false;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio")); 
	AudioComponent->SetupAttachment(GetRootComponent()); 
	AudioComponent->bCanPlayMultipleInstances = false; 
	AudioComponent->bAutoActivate = false; 
	AudioComponent->SetUISound(true);
}

void AWorldMusicInstance::SetMuted(const bool bInMuted, const bool bImmediate)
{
	if (bInMuted == bMuted) return;
	bMuted = bInMuted;
	if (bMuted)
	{
		if (bImmediate)
		{
			AudioComponent->SetPaused(true);
		}
		else
		{
			FTimerHandle Handle;
			AudioComponent->AdjustVolume(0.5f, 0.01f);
			GetWorldTimerManager().SetTimer(Handle, [this]()
			{
				AudioComponent->SetPaused(true);
			}, 0.6f, false);
		}
	}
	else
	{
		AudioComponent->SetPaused(false);
		AudioComponent->AdjustVolume(bImmediate ? 0.01f : 0.5f, 1.0f);
	}
}

void AWorldMusicInstance::OnChannelUpdate(const EMusicChannel InChannel, const bool bInMuted, const bool bImmediate)
{
	if (bOverlapping && MusicManager && InChannel == Channel)
	{
		SetMuted(bMuted, bImmediate);
	}
}

void AWorldMusicInstance::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (OtherActor == UGameplayStatics::GetPlayerPawn(this, 0))
	{
		bOverlapping = true;
		if (MusicManager && !MusicManager->IsChannelMuted(Channel))
		{
			SetMuted(false);
		}
	}
}

void AWorldMusicInstance::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	if (OtherActor == UGameplayStatics::GetPlayerPawn(this, 0))
	{
		bOverlapping = false;
		SetMuted(true);
	}
}

void AWorldMusicInstance::BeginPlay()
{
	Super::BeginPlay();
	MusicManager = AMusicManager::Get(this);
	if (MusicManager)
	{
		MusicManager->OnChannelStateChanged.AddUObject(this, &AWorldMusicInstance::OnChannelUpdate);
	}

	AudioComponent->SetSound(Sound);
	AudioComponent->SetVolumeMultiplier(Volume);
	
	AudioComponent->FadeIn(10.0f);
	AudioComponent->SetPaused(true);
}
