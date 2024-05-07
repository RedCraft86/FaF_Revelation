﻿// Copyright (C) RedCraft86. All Rights Reserved.

#include "FRGameState.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FRSettings.h"

#define CREATE_AUDIO_COMPONENT(Name) \
	Name = CreateDefaultSubobject<UAudioComponent>(#Name); \
	Name->SetupAttachment(GetRootComponent()); \
	Name->bCanPlayMultipleInstances = false; \
	Name->bAutoActivate = false; \
	Name->SetUISound(true);

AFRGameStateBase::AFRGameStateBase()
{
	CREATE_AUDIO_COMPONENT(WorldMusicAudio);
	CREATE_AUDIO_COMPONENT(SuspicionMusicAudio);
	CREATE_AUDIO_COMPONENT(ChaseMusicAudio);
	CREATE_AUDIO_COMPONENT(SearchMusicAudio);

	EnemyModeToAudio = {
		{EEnemyAIMode::None, WorldMusicAudio},
		{EEnemyAIMode::Suspicion, SuspicionMusicAudio},
		{EEnemyAIMode::Chase, ChaseMusicAudio},
		{EEnemyAIMode::Search, SearchMusicAudio}
	};
}

void AFRGameStateBase::SetGameMusic(const FGameMusicID InMusicID)
{
	StopGameMusic();
	MusicMode = EEnemyAIMode::None;
	MusicTracks = InMusicID.GetMusicData();

	for (const EEnemyAIMode Mode : TEnumRange<EEnemyAIMode>())
	{
		UAudioComponent* Comp = EnemyModeToAudio.FindRef(Mode);
		if (!Comp) continue;

		const FGameMusicTypeData* Data = MusicTracks.EnemyModeToTracks.Find(Mode);
		if (Data && !Data->Music.IsNull()) continue;

		Comp->SetSound(Data->Music.LoadSynchronous());
		if (Mode == MusicMode)
		{
			Comp->FadeIn(Data->TransitionTime);
		}
		else
		{
			Comp->FadeIn(0.0f, 0.05f);
			Comp->SetPaused(true);
		}
	}
}

void AFRGameStateBase::SetMusicMode(const EEnemyAIMode InMusicMode)
{
	if (MusicMode == InMusicMode) return;

	float Time = 0.0f;
	MusicMode = InMusicMode;
	for (const EEnemyAIMode Mode : TEnumRange<EEnemyAIMode>())
	{
		UAudioComponent* Comp = EnemyModeToAudio.FindRef(Mode);
		if (!Comp || !Comp->GetSound()) continue;

		const FGameMusicTypeData TypeData = MusicTracks.EnemyModeToTracks.FindRef(Mode);
		if (TypeData.TransitionTime > Time)
		{
			Time = TypeData.TransitionTime;
		}
		
		if (Mode == MusicMode)
		{
			Comp->SetPaused(false);
			if (TypeData.bRestartOnTransition)
			{
				Comp->FadeIn(TypeData.TransitionTime, 1.0f);
			}
			else
			{
				Comp->AdjustVolume(TypeData.TransitionTime, 1.0f);
			}
		}
		else
		{
			Comp->AdjustVolume(TypeData.TransitionTime * 0.5f, 0.05f);
		}
	}

	GetWorldTimerManager().ClearTimer(FadeTimer);
	GetWorldTimerManager().SetTimer(FadeTimer, [this]()
	{
		for (const EEnemyAIMode Mode : TEnumRange<EEnemyAIMode>())
		{
			if (UAudioComponent* Comp = EnemyModeToAudio.FindRef(Mode))
			{
				Comp->SetPaused(Mode != MusicMode);
			}
		}
	}, Time * 0.5f, false);
}

void AFRGameStateBase::StopGameMusic() const
{
	for (const EEnemyAIMode Mode : TEnumRange<EEnemyAIMode>())
	{
		if (UAudioComponent* Comp = EnemyModeToAudio.FindRef(Mode))
		{
			Comp->SetPaused(false);
			Comp->Stop();
		}
	}
}

void AFRGameStateBase::BeginPlay()
{
	Super::BeginPlay();
	if (const UFRSettings* Settings = FRSettings)
	{
		if (!Settings->IsGameplayMap(this))
			SetGameMusic(Settings->DefaultMusicID);
	}
}

AFRGameStateBase* AFRGameStateBase::GetGameStateSmart(const UObject* WorldContextObject, const TSubclassOf<AFRGameStateBase> Class)
{
	AFRGameStateBase* Obj = Cast<AFRGameStateBase>(UGameplayStatics::GetGameState(WorldContextObject));
	return Obj && Obj->IsA(Class) ? Obj : nullptr;
}

EGTValidPins AFRGameStateBase::GetGameStateChecked(AFRGameStateBase*& OutObject, const UObject* WorldContextObject, const TSubclassOf<AFRGameStateBase> Class)
{
	OutObject = GetGameStateSmart(WorldContextObject, Class);
	return IsValid(OutObject) ? EGTValidPins::Valid : EGTValidPins::NotValid;
}
