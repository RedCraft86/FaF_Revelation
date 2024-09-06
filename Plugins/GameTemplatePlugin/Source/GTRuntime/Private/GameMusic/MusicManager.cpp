// Copyright (C) RedCraft86. All Rights Reserved.

#include "GameMusic/MusicManager.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GTGameMode.h"
#include "GTSettings.h"

AMusicManager::AMusicManager()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 30.0f;
	PrimaryActorTick.bTickEvenWhenPaused = true;

#define CREATE_AUDIO_COMPONENT(Channel) \
	UAudioComponent* Channel = CreateDefaultSubobject<UAudioComponent>(#Channel TEXT("Audio")); \
	AudioComponents.Add(EMusicChannel::Channel, Channel);\
	Channel->SetupAttachment(GetRootComponent()); \
	Channel->bCanPlayMultipleInstances = false; \
	Channel->bAutoActivate = false; \
	Channel->SetUISound(true);

	CREATE_AUDIO_COMPONENT(Regular);
	CREATE_AUDIO_COMPONENT(Sensed);
	CREATE_AUDIO_COMPONENT(Chase);
	CREATE_AUDIO_COMPONENT(Search);
#undef CREATE_AUDIO_COMPONENT
}

void AMusicManager::SetChannelMuted(const EMusicChannel InChannel, const bool bMuted, const bool bImmediate)
{
	if (bChannelStates.FindRef(InChannel) == bMuted) return;
	OnChannelStateChanged.Broadcast(InChannel, bMuted, bImmediate);
	bChannelStates.Add(InChannel, bMuted);
	
	if (bMuted)
	{
		if (bImmediate)
		{
			if (UAudioComponent* Comp = AudioComponents.FindRef(InChannel))
			{
				Comp->SetPaused(true);
			}
			for (const FOneShotAudioData& Data : OneShotComponents)
			{
				if (Data.Channel == InChannel) Data.Component->Stop();
			}
			ClearStoppedOneShots();
		}
		else
		{
			UAudioComponent* Comp = AudioComponents.FindRef(InChannel);
			if (Comp) Comp->AdjustVolume(0.5f, 0.01f);
			
			for (const FOneShotAudioData& Data : OneShotComponents)
			{
				if (Data.Channel == InChannel)
					Data.Component->FadeOut(0.5f, 0.0f);
			}

			FTimerHandle Handle;
			GetWorldTimerManager().SetTimer(Handle, [this, Comp]()
			{
				ClearStoppedOneShots();
				if (Comp) Comp->SetPaused(true);
			}, 0.6f, false);
		}
	}
	else if (UAudioComponent* Comp = AudioComponents.FindRef(InChannel))
	{
		Comp->SetPaused(false);
		Comp->AdjustVolume(bImmediate ? 0.01f : 0.5f, 1.0f);
	}
}

void AMusicManager::MuteAllChannels(const bool bIncludeMainChannel, const bool bImmediate)
{
	if (bIncludeMainChannel) SetChannelMuted(EMusicChannel::Main, true, bImmediate);
	for (const EMusicChannel Channel : TEnumRange<EMusicChannel>())
	{
		SetChannelMuted(Channel, true, bImmediate);
	}
}

void AMusicManager::PlayOneShotAudio(USoundBase* InSound, const EMusicChannel InChannel, const float InVolume, const FString InOptionalID)
{
	if (!InSound || !bChannelStates.FindRef(InChannel)) return;
	FString OneShotAudioID = InOptionalID.Replace(TEXT(" "), TEXT("_"));
	if (OneShotAudioID.IsEmpty() || OneShotAudioID.Len() < 4) OneShotAudioID = FGuid::NewGuid().ToString();
	
	FOneShotAudioData NewData;
	NewData.Component = UGameplayStatics::CreateSound2D(this, InSound, InVolume);
	NewData.Component->FadeIn(0.5f);
	NewData.ID = OneShotAudioID;
	NewData.Channel = InChannel;
	OneShotComponents.Add(NewData);
}

void AMusicManager::SetBaseMusicData(UMusicDataBase* InData)
{
	if (InData && BaseMusicData != InData)
	{
		BaseMusicData = InData;
		for (const EMusicChannel Channel : TEnumRange<EMusicChannel>())
		{
			bChannelStates.Add(Channel, true);
			if (UAudioComponent* BaseComp = AudioComponents.FindRef(Channel))
			{
				BaseComp->bAutoDestroy = false;
				BaseComp->SetPaused(false);
				BaseComp->Stop();

				if (const FMusicChannelData* Data = BaseMusicData->Sounds.Find(Channel))
				{
					BaseComp->SetSound(Data->Sound);
					BaseComp->SetVolumeMultiplier(Data->Volume);
					BaseComp->FadeIn(10.0f);
					BaseComp->SetPaused(true);
				}
			}
		}

		SetChannelMuted(EMusicChannel::Regular, false, true);
	}
}

void AMusicManager::ClearStoppedOneShots()
{
	for (int i = OneShotComponents.Num() - 1; i >= 0; i--)
	{
		if (!OneShotComponents[i].Component || OneShotComponents[i].Component->GetPlayState() == EAudioComponentPlayState::Stopped)
		{
			if (OneShotComponents[i].Component) OneShotComponents[i].Component->DestroyComponent();
			OneShotComponents.RemoveAt(i);
		}
	}
}

void AMusicManager::BeginPlay()
{
	Super::BeginPlay();
	bChannelStates.Add(EMusicChannel::Main, false);
	for (const EMusicChannel Channel : TEnumRange<EMusicChannel>())
	{
		bChannelStates.Add(Channel, true);
	}

	SetBaseMusicData(UGTSettings::Get()->WorldMusic.LoadSynchronous());
}

void AMusicManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	ClearStoppedOneShots();
}

AMusicManager* AMusicManager::GetMusicManager(const UObject* WorldContextObject)
{
	if (AGTGameModeBase* GameMode = AGTGameModeBase::Get(WorldContextObject))
	{
		return GameMode->GetMusicManager();
	}

	return nullptr;
}
