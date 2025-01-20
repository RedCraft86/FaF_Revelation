// Copyright (C) RedCraft86. All Rights Reserved.

#include "DataTypes/OneShotDataTypes.h"
#include "Framework/ToroMusicManager.h"
#include "ToroRuntimeSettings.h"
#include "EnhancedCodeFlow.h"
#include "ToroRuntime.h"

#define GET_ONE_SHOT(Key) const FOneShotEntry SoundData = UOneShotDatabase::Get(Key)

#if WITH_EDITOR
void FOneShotEntry::Update()
{
	bIsLooping = IsValid(Sound.LoadSynchronous()) && Sound.LoadSynchronous()->IsLooping();
}
#endif

bool UOneShotDatabase::IsValidKey(const FGameplayTag& Key)
{
	if (!Key.IsValid()) return false;
	const UToroRuntimeSettings* Settings = UToroRuntimeSettings::Get();
	if (const UOneShotDatabase* Database = Settings ? Settings->OneShotDatabase.LoadSynchronous() : nullptr)
	{
		return Database->OneShots.Contains(Key);
	}
	
	return false;
}

FOneShotEntry UOneShotDatabase::Get(const FGameplayTag& Key)
{
	if (!Key.IsValid()) return {};
	const UToroRuntimeSettings* Settings = UToroRuntimeSettings::Get();
	if (const UOneShotDatabase* Database = Settings ? Settings->OneShotDatabase.LoadSynchronous() : nullptr)
	{
		return Database->OneShots.FindRef(Key);
	}
	
	return {};
}

#if WITH_EDITOR
void UOneShotDatabase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	for (TPair<FGameplayTag, FOneShotEntry>& OneShot : OneShots)
	{
		OneShot.Value.Update();
	}
}
#endif

void FOneShotLayer::Stop()
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

void FOneShotLayer::Restart()
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

void FOneShotLayer::SetPaused(const bool bInPaused)
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

void FOneShotLayer::AddInstigator(const UObject* InObject)
{
	Instigators.Add(InObject);
}

void FOneShotLayer::RemoveInstigator(const UObject* InObject)
{
	Instigators.Remove(InObject);
	if (Instigators.IsEmpty()) Stop();
}

bool FOneShotLayer::IsValidLayer() const
{
	return IsValid(Component) && !bStopping;
}

bool FOneShotLayer::CanRunFunctions() const
{
	return IsValid(Component) && IsValid(Owner)
		&& !FEnhancedCodeFlow::IsActionRunning(Owner, FadeHandle);
}

void FOneShotLayer::Initialize(AToroMusicManager* InOwner, const UObject* Instigator)
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
		Component->OnAudioFinishedNative.AddRaw(this, &FOneShotLayer::OnAudioFinished);
		Component->FadeIn(SoundData.FadeTimes.X, 1.0f, SoundData.GetStartTime());
		bAutoDestroy = true;
	}
	else Owner->CleanOneShotTracks();
}

void FOneShotLayer::OnAudioFinished(UAudioComponent* Comp)
{
	if (bAutoDestroy && Component)
	{
		Component->DestroyComponent();
		Component = nullptr;
	}
	
	if (Owner) Owner->CleanOneShotTracks();
}
