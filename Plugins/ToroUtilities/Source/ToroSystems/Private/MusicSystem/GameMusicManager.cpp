// Copyright (C) RedCraft86. All Rights Reserved.

#include "MusicSystem/GameMusicManager.h"
#include "Framework/ToroPlayerCharacter.h"
#include "Framework/ToroPlayerState.h"
#include "Framework/ToroGameState.h"
#include "Framework/ToroGameMode.h"
#include "ToroSystems.h"

UGameMusicManager::UGameMusicManager()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	SetUISound(true);
	bAutoActivate = false;
	bCanPlayMultipleInstances = false;
	bIsMusic = true;
}
UGameMusicManager* UGameMusicManager::GetMusicManager(const UObject* ContextObject, const int32 PlayerIndex)
{
	FIND_GENERAL_COMPONENT(UGameMusicManager)
}

void UGameMusicManager::ChangeMainTheme(UMetaSoundSource* InTheme)
{
	if (InTheme && Sound != InTheme)
	{
		if (bIsPaused)
		{
			Stop();
			SetPaused(false);
			SetSound(InTheme);
			FadeIn(1.0f);
		}
		else
		{
			FadeOut(1.0f, 0.0f);
			GetWorld()->GetTimerManager().ClearTimer(ChangeTimer);
			GetWorld()->GetTimerManager().SetTimer(ChangeTimer, [this, InTheme]()
			{
				SetSound(InTheme);
				FadeIn(1.0f);
			}, 1.0f, false);
		}

		SetThemeState(0);
		SetThemeIntensity(0.0f);
		OnThemeChanged.Broadcast();
	}
}

void UGameMusicManager::SetThemeState(const uint8 InState)
{
	if (StateIdx != InState)
	{
		StateIdx = InState;
		GetParamInterface()->SetIntParameter(TEXT("State"), InState);
		OnStateChanged.Broadcast(StateIdx);
	}
}

void UGameMusicManager::SetThemeIntensity(const float InIntensity)
{
	GetParamInterface()->SetFloatParameter(TEXT("Intensity"), InIntensity);
}

void UGameMusicManager::AddDipRequest(const UObject* InObject)
{
	if (InObject)
	{
		DipRequests.Add(InObject);
		UpdateDipState();
	}
}

void UGameMusicManager::RemoveDipRequest(const UObject* InObject)
{
	if (InObject)
	{
		if (DipRequests.Remove(InObject) > 0)
		{
			UpdateDipState();
		}
	}
}

void UGameMusicManager::UpdateDipState()
{
	for (auto It = DipRequests.CreateIterator(); It; ++It)
	{
		if (!It->IsValid()) It.RemoveCurrent();
	}
	const bool bDipped = !DipRequests.IsEmpty();
	GetParamInterface()->SetBoolParameter(TEXT("Dipped"), bDipped);
	if (bDipped) OnAudioDipped.Broadcast();
}

void UGameMusicManager::BeginPlay()
{
	Super::BeginPlay();
	if (UMetaSoundSource* Default = UToroSysSettings::Get()->DefaultTheme.LoadSynchronous())
	{
		SetSound(Default);
		FadeIn(1.0f);
	}
}

void UGameMusicManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc)
{
	Super::TickComponent(DeltaTime, TickType, TickFunc);
	if (const bool bIsGamePaused = GetWorld()->IsPaused(); bGamePaused != bIsGamePaused)
	{
		bGamePaused = bIsGamePaused;
		bGamePaused ? AddDipRequest(this) : RemoveDipRequest(this);
	}
}

IAudioParameterControllerInterface* UGameMusicManager::GetParamInterface()
{
	return Cast<IAudioParameterControllerInterface>(this);
}
