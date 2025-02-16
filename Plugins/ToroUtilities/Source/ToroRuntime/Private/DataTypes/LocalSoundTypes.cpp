// Copyright (C) RedCraft86. All Rights Reserved.

#include "DataTypes/LocalSoundTypes.h"
#include "Framework/ToroMusicManager.h"
#include "ToroRuntimeSettings.h"
#include "EnhancedCodeFlow.h"
#include "ToroRuntime.h"

#define GET_ONE_SHOT(Key) const FLocalSoundEntry SoundData = ULocalSoundDatabase::Get(Key)

UE_DEFINE_GAMEPLAY_TAG(Tag_LocalSound, "LocalSound");

#if WITH_EDITOR
void FLocalSoundEntry::Update()
{
	// ReSharper disable once CppExpressionWithoutSideEffects
	Sound.LoadSynchronous();
	Fades.X = FMath::Max(0.1f, Fades.X);
	Fades.Y = FMath::Max(0.1f, Fades.Y);
	Fades.Z = FMath::Max(0.1f, Fades.Z);
	Volume = FMath::Max(0.1f, Volume);
	StartRange.X = FMath::Max(0.0f, StartRange.X);
	StartRange.Y = FMath::Max(0.0f, StartRange.Y);
}
#endif

bool ULocalSoundDatabase::IsKeyValid(const FGameplayTag& Key) const
{
	return Key.IsValid() && Key != Tag_LocalSound
		&& Entries.Contains(Key) && Entries.FindRef(Key).IsValidData();
}

#if WITH_EDITOR
uint8 ULocalSoundDatabase::GetValidCount() const
{
	uint8 Count = 0;
	for (const TPair<FGameplayTag, FLocalSoundEntry>& Entry : Entries)
	{
		if (Entry.Key.IsValid() && Entry.Key != Tag_LocalSound && Entry.Value.IsValidData())
		{
			Count++;
		}
	}

	return Count;
}
#endif

bool ULocalSoundDatabase::IsValidKey(const FGameplayTag& Key)
{
	if (!Key.IsValid()) return false;
	if (const ULocalSoundDatabase* Database = UToroRuntimeSettings::Get()->LocalSoundDatabase.LoadSynchronous())
	{
		return Database->IsKeyValid(Key);
	}
	
	return false;
}

FLocalSoundEntry ULocalSoundDatabase::Get(const FGameplayTag& Key)
{
	if (!Key.IsValid()) return {};
	if (const ULocalSoundDatabase* Database = UToroRuntimeSettings::Get()->LocalSoundDatabase.LoadSynchronous())
	{
		return Database->Entries.FindRef(Key);
	}
	
	return {};
}

#if WITH_EDITOR
void ULocalSoundDatabase::UpdateSounds()
{
	for (TPair<FGameplayTag, FLocalSoundEntry>& Entry : Entries)
	{
		Entry.Value.Update();
		if (!Entry.Key.IsValid() || Entry.Key == Tag_LocalSound)
		{
			Entry.Value.Label = TEXT("INVALID Tag");
		}
		else if (!Entry.Value.IsValidData())
		{
			Entry.Value.Label = TEXT("INVALID Data");
		}
		else
		{
			Entry.Value.Label = Entry.Value.Sound.GetAssetName();
		}
	}
}

void ULocalSoundDatabase::PostLoad()
{
	Super::PostLoad();
	UpdateSounds();
}

void ULocalSoundDatabase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	UpdateSounds();
}
#endif

void FLocalSoundLayer::Stop()
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
			Owner->CleanLocalSoundTracks();
	};
	
	bAutoDestroy = false;
	if (bPaused)
	{
		StopFunc();
	}
	else
	{
		GET_ONE_SHOT(SoundID);
		Component->FadeOut(SoundData.Fades.Y, 0.0f);
		FadeHandle = FEnhancedCodeFlow::Delay(Owner, SoundData.Fades.Y, StopFunc);
	}
}

void FLocalSoundLayer::Restart()
{
	if (!CanRunFunctions()) return;

	bAutoDestroy = false;
	GET_ONE_SHOT(SoundID);
	Component->FadeOut(SoundData.Fades.Y, 0.0f);
	FadeHandle = FEnhancedCodeFlow::Delay(Owner, SoundData.Fades.Y, [this, SoundData]()
	{
		if (Component)
		{
			Component->FadeIn(SoundData.Fades.X, 1.0f, SoundData.GetStartTime());
		}
		bAutoDestroy = true;
	});
}

void FLocalSoundLayer::SetPaused(const bool bInPaused)
{
	if (!CanRunFunctions()) return;
	if (bPaused != bInPaused)
	{
		bPaused = bInPaused;
		GET_ONE_SHOT(SoundID);
		if (bInPaused)
		{
			Component->AdjustVolume(SoundData.Fades.Z, 0.05f);
			PauseHandle = FEnhancedCodeFlow::Delay(Owner, SoundData.Fades.Z, [this]()
			{
				if (Component) Component->SetPaused(true);
			});
		}
		else
		{
			Component->SetPaused(false);
			Component->AdjustVolume(SoundData.Fades.Z, 1.0f);
		}
	}
}

void FLocalSoundLayer::AddInstigator(const UObject* InObject)
{
	Instigators.Add(InObject);
}

void FLocalSoundLayer::RemoveInstigator(const UObject* InObject)
{
	Instigators.Remove(InObject);
	if (Instigators.IsEmpty()) Stop();
}

bool FLocalSoundLayer::IsLooping() const
{
	return Component && Component->Sound ? Component->Sound->IsLooping() : false;
}

bool FLocalSoundLayer::IsValidLayer() const
{
	return IsValid(Component) && !bStopping;
}

bool FLocalSoundLayer::CanRunFunctions() const
{
	return IsValid(Component) && IsValid(Owner)
		&& !FEnhancedCodeFlow::IsActionRunning(Owner, FadeHandle);
}

void FLocalSoundLayer::Initialize(AToroMusicManager* InOwner, const UObject* Instigator)
{
	Owner = InOwner;
	Instigators = {Instigator};
	if (!Owner || Instigators.IsEmpty()) return;
	
	GET_ONE_SHOT(SoundID);
	if (!SoundData.IsValidData())
	{
		UE_LOG(LogToroRuntime, Error, TEXT("Attempting to play one-shot %s but it is invalid!"), *SoundID.ToString())
		Owner->CleanLocalSoundTracks();
		return;
	}
	
	Component = UGameplayStatics::CreateSound2D(Owner, SoundData.Sound.LoadSynchronous(), SoundData.Volume);
	if (Component)
	{
		Component->bIsMusic = true;
		Component->bAutoDestroy = false;
		Component->OnAudioFinishedNative.AddRaw(this, &FLocalSoundLayer::OnAudioFinished);
		Component->FadeIn(SoundData.Fades.X, 1.0f, SoundData.GetStartTime());
		bAutoDestroy = true;
	}
	else Owner->CleanLocalSoundTracks();
}

void FLocalSoundLayer::OnAudioFinished(UAudioComponent* Comp)
{
	if (bAutoDestroy && Component)
	{
		Component->DestroyComponent();
		Component = nullptr;
	}
	
	if (Owner) Owner->CleanLocalSoundTracks();
}
