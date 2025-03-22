// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroMusicManager.h"
#include "Components/AudioComponent.h"
#include "EnhancedCodeFlow.h"
#include "ToroSettings.h"

AToroMusicManager::AToroMusicManager() : bThemeMuted(false)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;
	PrimaryActorTick.TickInterval = 1.0f;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);

	ThemeComponent = CreateDefaultSubobject<UAudioComponent>("ThemeComponent");
	ThemeComponent->SetupAttachment(SceneRoot);
	ThemeComponent->SetUISound(true);
	ThemeComponent->bCanPlayMultipleInstances = false;
	ThemeComponent->bAutoActivate = false;
	ThemeComponent->bIsMusic = true;
}

EToroValidPins AToroMusicManager::GetMusicManager(AToroMusicManager*& OutObject,
	const UObject* ContextObject, const TSubclassOf<AToroMusicManager>& Class)
{
	AToroMusicManager* Obj = Get<AToroMusicManager>(ContextObject);
	OutObject = IsValid(Obj) && (!Class || Obj->IsA(Class)) ? Obj : nullptr;
	return IsValid(OutObject) ? EToroValidPins::Valid : EToroValidPins::NotValid;
}

bool AToroMusicManager::ChangeMainTheme(UMetaSoundSource* NewTheme)
{
	if (!NewTheme || NewTheme == MainTheme) return false;
	if (!MainTheme)
	{
		MainTheme = NewTheme;
		ThemeComponent->SetSound(NewTheme);
		if (!bThemeMuted) ThemeComponent->FadeIn(1.0f);
		return true;
	}

	if (FFlow::IsActionRunning(this, ChangeHandle))
		return false;

	if (!bThemeMuted) ThemeComponent->FadeOut(1.0f, 0.0f);
	ChangeHandle = FFlow::Delay(this, 1.0f, [this, NewTheme]()
	{
		MainTheme = NewTheme;
		ThemeComponent->SetSound(NewTheme);
		if (!bThemeMuted) ThemeComponent->FadeIn(1.0f);
	});

	SetHidingState(0.0f);
	SetThemeState(0);
	return true;
}

void AToroMusicManager::SetThemeMuted(const bool bMuted, const float Duration)
{
	if (bThemeMuted != bMuted)
	{
		bThemeMuted = bMuted;
		const float Time = FMath::Abs(Duration);
		FFlow::StopAction(this, MuteHandle);
		if (FMath::IsNearlyZero(Time))
		{
			ThemeComponent->SetPaused(bThemeMuted);
		}
		else
		{
			if (bThemeMuted)
			{
				ThemeComponent->AdjustVolume(Time, 0.1f);
				MuteHandle = FFlow::Delay(this, Time, [this]()
				{
					ThemeComponent->SetPaused(true);
				});
			}
			else
			{
				ThemeComponent->SetPaused(false);
				ThemeComponent->AdjustVolume(Time, 1.0f);
			}
		}
	}
}

void AToroMusicManager::SetDipAudio(const bool bDipAudio) const
{
	GetSoundParamInterface()->SetBoolParameter(TEXT("DipAudio"), bDipAudio);
}

void AToroMusicManager::SetHidingState(const bool bHiding) const
{
	GetSoundParamInterface()->SetBoolParameter(TEXT("Hiding"), bHiding);
}

void AToroMusicManager::SetThemeState(const uint8 InState) const
{
	GetSoundParamInterface()->SetIntParameter(TEXT("State"), InState);
}

bool AToroMusicManager::PlayLayer(const UObject* InInstigator, const FGameplayTag InSoundID)
{
	if (!ULocalSoundDatabase::IsValidKey(InSoundID)) return false;
	if (FLocalSoundLayer* Layer = LocalSoundLayers.Find(InSoundID))
	{
		if (Layer->IsValidLayer())
		{
			Layer->AddInstigator(InInstigator);
			return false;
		}
	}

	FLocalSoundLayer NewLayer(InSoundID);
	NewLayer.Initialize(this, InInstigator);
	LocalSoundLayers.Emplace(InSoundID, NewLayer);
	SetActorTickEnabled(true);
	return true;
}

bool AToroMusicManager::StopLayer(const UObject* InInstigator, const FGameplayTag InSoundID)
{
	if (FLocalSoundLayer* Layer = LocalSoundLayers.Find(InSoundID))
	{
		if (Layer->IsValidLayer())
		{
			Layer->RemoveInstigator(InInstigator);
			return true;
		}
	}
	return false;
}

bool AToroMusicManager::StopLayerIfLooping(const UObject* InInstigator, const FGameplayTag InSoundID)
{
	if (FLocalSoundLayer* Layer = LocalSoundLayers.Find(InSoundID))
	{
		if (Layer->IsValidLayer() && Layer->IsLooping())
		{
			Layer->RemoveInstigator(InInstigator);
			return true;
		}
	}

	return false;
}

bool AToroMusicManager::RestartLayer(const FGameplayTag InSoundID)
{
	if (FLocalSoundLayer* Layer = LocalSoundLayers.Find(InSoundID))
	{
		if (Layer->IsValidLayer())
		{
			Layer->Restart();
			return true;
		}
	}

	return false;
}

bool AToroMusicManager::SetLayerPaused(const FGameplayTag InSoundID, const bool bPaused)
{
	if (FLocalSoundLayer* Layer = LocalSoundLayers.Find(InSoundID))
	{
		if (Layer->IsValidLayer())
		{
			Layer->SetPaused(bPaused);
			return true;
		}
	}

	return false;
}

void AToroMusicManager::CleanLocalSoundTracks()
{
	for (auto It = LocalSoundLayers.CreateIterator(); It; ++It)
	{
		if (!It.Value().IsValidLayer())
		{
			It.RemoveCurrent();
		}
	}

	if (LocalSoundLayers.IsEmpty())
	{
		SetActorTickEnabled(false);
	}
}

IAudioParameterControllerInterface* AToroMusicManager::GetSoundParamInterface() const
{
	return Cast<IAudioParameterControllerInterface>(ThemeComponent);
}

void AToroMusicManager::BeginPlay()
{
	Super::BeginPlay();
	const UToroSettings* Settings = UToroSettings::Get();
	if (!Settings->IsOnGameplayMap(this))
	{
		FFlow::Delay(this, 1.0f, [this, Settings]()
		{
			ChangeMainTheme(Settings->DefaultTheme.LoadSynchronous());
		});
	}
}

void AToroMusicManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	CleanLocalSoundTracks();
}
