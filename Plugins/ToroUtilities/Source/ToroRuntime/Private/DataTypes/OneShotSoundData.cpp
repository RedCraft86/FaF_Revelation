// Copyright (C) RedCraft86. All Rights Reserved.

#include "DataTypes/OneShotSoundData.h"
#include "Framework/ToroMusicManager.h"
#include "ToroRuntimeSettings.h"
#include "EnhancedCodeFlow.h"
#include "ToroRuntime.h"

#define GET_ONE_SHOT(Key) const FOneShotSoundData SoundData = FOneShotSoundData::Get(Key)

void FOneShotSoundData::OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName)
{
	Super::OnDataTableChanged(InDataTable, InRowName);
	bIsLooping = IsValid(Sound.LoadSynchronous()) && Sound.LoadSynchronous()->IsLooping();
}

bool FOneShotSoundData::IsValidKey(const FName& Key)
{
	if (Key.IsNone()) return false;
	const UToroRuntimeSettings* Settings = UToroRuntimeSettings::Get();
	if (UDataTable* Table = Settings ? Settings->OneShotSoundTable.LoadSynchronous() : nullptr)
	{
		return Table->GetRowMap().Contains(Key);
	}

	return false;
}

FOneShotSoundData FOneShotSoundData::Get(const FName& Key)
{
	if (Key.IsNone()) return {};
	const UToroRuntimeSettings* Settings = UToroRuntimeSettings::Get();
	if (UDataTable* Table = Settings ? Settings->OneShotSoundTable.LoadSynchronous() : nullptr)
	{
		if (Table->GetRowMap().Contains(Key))
		{
			return *Table->FindRow<FOneShotSoundData>(Key, Table->GetName());
		}
	}

	return {};
}

void FOneShotSoundLayer::Stop()
{
	if (!IsValid(Component) || !IsValid(Owner)) return;
	FEnhancedCodeFlow::StopAction(Owner, PauseHandle);
	FEnhancedCodeFlow::StopAction(Owner, FadeHandle);
	
	auto StopFunc = [this]()
	{
		bStopping = true;
		if (Component)
		{
			Component->DestroyComponent();
			Component = nullptr;
		}
		if (Owner)
			Owner->CleanOneShotTracks();
	};
	
	bAutoDestroy = false;
	if (bPaused)
	{
		StopFunc();
	}
	else
	{
		GET_ONE_SHOT(SoundID);
		Component->FadeOut(SoundData.FadeTimes.Y, 0.0f);
		FadeHandle = FEnhancedCodeFlow::Delay(Owner, SoundData.FadeTimes.Y, StopFunc);
	}
}

void FOneShotSoundLayer::Restart()
{
	if (!CanRunFunctions()) return;

	bAutoDestroy = false;
	GET_ONE_SHOT(SoundID);
	Component->FadeOut(SoundData.FadeTimes.Y, 0.0f);
	FadeHandle = FEnhancedCodeFlow::Delay(Owner, SoundData.FadeTimes.Y, [this, SoundData]()
	{
		if (Component)
		{
			Component->FadeIn(SoundData.FadeTimes.X, 1.0f, SoundData.GetStartTime());
		}
		bAutoDestroy = true;
	});
}

void FOneShotSoundLayer::SetPaused(const bool bInPaused)
{
	if (!CanRunFunctions()) return;
	if (bPaused != bInPaused)
	{
		bPaused = bInPaused;
		GET_ONE_SHOT(SoundID);
		if (bInPaused)
		{
			Component->AdjustVolume(SoundData.FadeTimes.Z, 0.05f);
			PauseHandle = FEnhancedCodeFlow::Delay(Owner, SoundData.FadeTimes.Z, [this]()
			{
				if (Component) Component->SetPaused(true);
			});
		}
		else
		{
			Component->SetPaused(false);
			Component->AdjustVolume(SoundData.FadeTimes.Z, 1.0f);
		}
	}
}

void FOneShotSoundLayer::AddInstigator(const UObject* InObject)
{
	Instigators.Add(InObject);
}

void FOneShotSoundLayer::RemoveInstigator(const UObject* InObject)
{
	Instigators.Remove(InObject);
	if (Instigators.IsEmpty()) Stop();
}

bool FOneShotSoundLayer::IsValidLayer() const
{
	return IsValid(Component) && !bStopping;
}

bool FOneShotSoundLayer::CanRunFunctions() const
{
	return IsValid(Component) && IsValid(Owner)
		&& !FEnhancedCodeFlow::IsActionRunning(Owner, FadeHandle);
}

void FOneShotSoundLayer::Initialize(AToroMusicManager* InOwner, const UObject* Instigator)
{
	Owner = InOwner;
	Instigators = {Instigator};
	if (!Owner || Instigators.IsEmpty()) return;
	
	GET_ONE_SHOT(SoundID);
	if (!SoundData.IsValidData())
	{
		UE_LOG(LogToroRuntime, Error, TEXT("Attempting to play one-shot %s but it is invalid!"), *SoundID.ToString())
		Owner->CleanOneShotTracks();
		return;
	}
	
	Component = UGameplayStatics::CreateSound2D(Owner, SoundData.Sound.LoadSynchronous(), SoundData.Volume);
	if (Component)
	{
		Component->bIsMusic = true;
		Component->bAutoDestroy = false;
		Component->OnAudioFinishedNative.AddRaw(this, &FOneShotSoundLayer::OnAudioFinished);
		Component->FadeIn(SoundData.FadeTimes.X, 1.0f, SoundData.GetStartTime());
		bAutoDestroy = true;
	}
	else Owner->CleanOneShotTracks();
}

void FOneShotSoundLayer::OnAudioFinished(UAudioComponent* Comp)
{
	if (bAutoDestroy && Component)
	{
		Component->DestroyComponent();
		Component = nullptr;
	}
	
	if (Owner) Owner->CleanOneShotTracks();
}
