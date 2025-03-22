// Copyright (C) RedCraft86. All Rights Reserved.

#include "DataTypes/LocalSoundTypes.h"
#include "Framework/ToroMusicManager.h"
#include "Components/AudioComponent.h"
#include "EnhancedCodeFlow.h"
#include "ToroUtilities.h"
#include "ToroSettings.h"

UE_DEFINE_GAMEPLAY_TAG_BASE(LocalSound)

#define GET_ONE_SHOT(Key) const FLocalSoundEntry SoundData = ULocalSoundDatabase::Get(Key)

bool ULocalSoundDatabase::IsKeyValid(const FGameplayTag& Key) const
{
	return VerifyLocalSoundTag(Key) && Entries.Contains(Key) && Entries.FindRef(Key).IsValidData();
}

const FLocalSoundEntry* ULocalSoundDatabase::Find(const FGameplayTag& Key) const
{
	return IsKeyValid(Key) ? Entries.Find(Key) : nullptr;
}

bool ULocalSoundDatabase::IsValidKey(const FGameplayTag& Key)
{
	const ULocalSoundDatabase* Database = UToroSettings::Get()->LocalSoundDatabase.LoadSynchronous();
	return Database ? Database->IsKeyValid(Key) : false;
}

FLocalSoundEntry ULocalSoundDatabase::Get(const FGameplayTag& Key)
{
	const ULocalSoundDatabase* Database = UToroSettings::Get()->LocalSoundDatabase.LoadSynchronous();
	const FLocalSoundEntry* Entry = Database ? Database->Find(Key) : nullptr;
	return Entry ? *Entry : FLocalSoundEntry();
}

#if WITH_EDITOR
int32 ULocalSoundDatabase::GetValidData() const
{
	int32 Count = 0;
	for (const TPair<FGameplayTag, FLocalSoundEntry>& Entry : Entries)
	{
		if (VerifyLocalSoundTag(Entry.Key) && Entry.Value.IsValidData())
		{
			Count++;
		}
	}
	return Count;
}

void ULocalSoundDatabase::UpdateData()
{
	Super::UpdateData();
	for (TPair<FGameplayTag, FLocalSoundEntry>& Entry : Entries)
	{
		Entry.Value.Update();
		if (!VerifyLocalSoundTag(Entry.Key))
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
#endif

void FLocalSoundLayer::Stop()
{
	if (!IsValid(Component) || !IsValid(Owner)) return;
	FFlow::StopAction(Owner, PauseHandle);
	FFlow::StopAction(Owner, FadeHandle);
	
	auto StopFunc = [this]()
	{
		bStopping = true;
		if (Component)
		{
			Component->DestroyComponent();
			Component = nullptr;
		}
		if (Owner)
		{
			Owner->CleanLocalSoundTracks();
		}
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
		FadeHandle = FFlow::Delay(Owner, SoundData.Fades.Y, StopFunc);
	}
}

void FLocalSoundLayer::Restart()
{
	if (!CanRunFunctions())
		return;

	bAutoDestroy = false;
	GET_ONE_SHOT(SoundID);
	Component->FadeOut(SoundData.Fades.Y, 0.0f);
	FadeHandle = FFlow::Delay(Owner, SoundData.Fades.Y, [this, SoundData]()
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
			PauseHandle = FFlow::Delay(Owner, SoundData.Fades.Z, [this]()
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
	return IsValid(Component) && IsValid(Owner) && !FFlow::IsActionRunning(Owner, FadeHandle);
}

void FLocalSoundLayer::Initialize(AToroMusicManager* InOwner, const UObject* Instigator)
{
	Owner = InOwner;
	Instigators = {Instigator};
	if (!Owner || Instigators.IsEmpty()) return;

	GET_ONE_SHOT(SoundID);
	if (!SoundData.IsValidData())
	{
		UE_LOG(LogToroUtilities, Error, TEXT("Attempting to play one-shot %s but it is invalid!"), *SoundID.ToString())
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
