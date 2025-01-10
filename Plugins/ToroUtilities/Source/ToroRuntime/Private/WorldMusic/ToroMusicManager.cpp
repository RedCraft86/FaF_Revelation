// Copyright (C) RedCraft86. All Rights Reserved.

#include "WorldMusic/WorldMusicManager.h"
#include "ToroRuntimeSettings.h"
#include "EnhancedCodeFlow.h"

void FOneShotMusicLayer::Stop(const float FadeTime)
{
	if (!IsValid(Component) || !IsValid(Owner)) return;
	FEnhancedCodeFlow::StopAction(Owner, PauseHandle);
	FEnhancedCodeFlow::StopAction(Owner, FadeHandle);
	
	auto StopFunc = [this]()
	{
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
		Component->FadeOut(FadeTime, 0.0f);
		FadeHandle = FEnhancedCodeFlow::Delay(Owner, FadeTime, StopFunc);
	}
}

void FOneShotMusicLayer::Restart(const float FadeTime)
{
	if (!CanRunFunctions()) return;

	bAutoDestroy = false;
	Component->FadeOut(FadeTime, 0.0f);
	FadeHandle = FEnhancedCodeFlow::Delay(Owner, FadeTime, [this, FadeTime]()
	{
		if (Component) Component->FadeIn(FadeTime, 1.0f,
			FMath::RandRange(Start.GetMin(), Start.GetMax()));
		
		bAutoDestroy = true;
	});
}

void FOneShotMusicLayer::SetPaused(const float FadeTime, const bool bInPaused)
{
	if (!CanRunFunctions()) return;
	if (bPaused != bInPaused)
	{
		bPaused = bInPaused;
		if (bInPaused)
		{
			Component->AdjustVolume(FadeTime, 0.05f);
			PauseHandle = FEnhancedCodeFlow::Delay(Owner, FadeTime, [this]()
			{
				if (Component) Component->SetPaused(true);
			});
		}
		else
		{
			Component->SetPaused(false);
			Component->AdjustVolume(FadeTime, 1.0f);
		}
	}
}

bool FOneShotMusicLayer::CanRunFunctions() const
{
	return IsValid(Component) && IsValid(Owner)
		&& !FEnhancedCodeFlow::IsActionRunning(Owner, FadeHandle);
}

void FOneShotMusicLayer::Initialize(const float FadeTime)
{
	if (Component)
	{
		Component->bIsMusic = true;
		Component->bAutoDestroy = false;
		Component->OnAudioFinishedNative.AddRaw(this, &FOneShotMusicLayer::OnAudioFinished);
		Component->FadeIn(FadeTime, 1.0f, FMath::RandRange(Start.GetMin(), Start.GetMax()));
		bAutoDestroy = true;
	}
	else if (Owner) Owner->CleanOneShotTracks();
}

void FOneShotMusicLayer::OnAudioFinished(UAudioComponent* Comp)
{
	if (bAutoDestroy && Component)
	{
		Component->DestroyComponent();
		Component = nullptr;
	}
	
	if (Owner) Owner->CleanOneShotTracks();
}

AWorldMusicManager::AWorldMusicManager()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;
	PrimaryActorTick.TickInterval = 1.0f;
	
	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);

	MainThemeComponent = CreateDefaultSubobject<UAudioComponent>("MainThemeComponent");
	MainThemeComponent->SetupAttachment(SceneRoot);
	MainThemeComponent->SetUISound(true);
	MainThemeComponent->bCanPlayMultipleInstances = false;
	MainThemeComponent->bAutoActivate = false;
	MainThemeComponent->bIsMusic = true;
}

EToroValidPins AWorldMusicManager::GetMusicManager(AWorldMusicManager*& OutObject,
	const UObject* WorldContextObject, const TSubclassOf<AWorldMusicManager>& Class)
{
	AWorldMusicManager* Obj = Cast<AWorldMusicManager>(UGameplayStatics::GetGameState(WorldContextObject));
	OutObject = IsValid(Obj) && (!Class || Obj->IsA(Class)) ? Obj : nullptr;
	return IsValid(OutObject) ? EToroValidPins::Valid : EToroValidPins::NotValid;
}

bool AWorldMusicManager::ChangeMainTheme(UMetaSoundSource* NewTheme)
{
	if (!NewTheme || NewTheme == MainTheme) return false;
	if (!MainTheme)
	{
		MainTheme = NewTheme;
		MainThemeComponent->SetSound(NewTheme);
		MainThemeComponent->FadeIn(1.0f);
		return true;
	}
	
	if (FEnhancedCodeFlow::IsActionRunning(this, ChangeHandle)) return false;

	MainThemeComponent->FadeOut(1.0f, 0.0f);
	ChangeHandle = FEnhancedCodeFlow::Delay(this, 1.0f, [this, NewTheme]()
	{
		MainTheme = NewTheme;
		MainThemeComponent->SetSound(NewTheme);
		MainThemeComponent->FadeIn(1.0f);
	});

	return true;
}

void AWorldMusicManager::SetThemeIntensity(const float InIntensity) const
{
	GetSoundParamInterface()->SetFloatParameter(TEXT("Intensity"), InIntensity);
}

void AWorldMusicManager::SetThemeState(const uint8 InState) const
{
	GetSoundParamInterface()->SetIntParameter(TEXT("State"), InState);
}

bool AWorldMusicManager::PlayLayer(USoundBase* Sound, const float FadeTime, const float Volume, const FVector2D& StartRange)
{
	if (!Sound) return false;
	if (const FOneShotMusicLayer* Layer = OneShotLayers.Find(Sound))
	{
		if (Layer->Component) return false;
	}
	
	FOneShotMusicLayer NewLayer;
	NewLayer.Component = UGameplayStatics::CreateSound2D(this, Sound, Volume);
	NewLayer.Owner = this;
	NewLayer.Start = StartRange;

	OneShotLayers.Remove(Sound);
	OneShotLayers.Emplace(Sound, NewLayer).Initialize(FadeTime);
	SetActorTickEnabled(true);
	return true;
}

bool AWorldMusicManager::StopLayer(const USoundBase* Sound, const float FadeTime)
{
	if (!Sound) return false;
	if (FOneShotMusicLayer* Layer = OneShotLayers.Find(Sound))
	{
		Layer->Stop(FMath::Max(0.0f, FadeTime));
		return true;
	}
	
	return false;
}

bool AWorldMusicManager::RestartLayer(const USoundBase* Sound, const float FadeTime)
{
	if (!Sound) return false;
	if (FOneShotMusicLayer* Layer = OneShotLayers.Find(Sound))
	{
		Layer->Restart(FMath::Max(0.0f, FadeTime));
		return true;
	}
	
	return false;
}

bool AWorldMusicManager::SetLayerPaused(const USoundBase* Sound, const float FadeTime, const bool bPaused)
{
	if (!Sound) return false;
	if (FOneShotMusicLayer* Layer = OneShotLayers.Find(Sound))
	{
		Layer->SetPaused(FMath::Max(0.0f, FadeTime), bPaused);
		return true;
	}
	
	return false;
}

void AWorldMusicManager::CleanOneShotTracks()
{
	if (OneShotLayers.IsEmpty())
	{
		SetActorTickEnabled(false);
	}
	
	for (auto It = OneShotLayers.CreateIterator(); It; ++It)
	{
		if (!It.Value().Component)
		{
			It.RemoveCurrent();
		}
	}
}

IAudioParameterControllerInterface* AWorldMusicManager::GetSoundParamInterface() const
{
	return Cast<IAudioParameterControllerInterface>(MainThemeComponent);
}

void AWorldMusicManager::BeginPlay()
{
	Super::BeginPlay();
	FEnhancedCodeFlow::Delay(this, 1.0f, [this]()
	{
		ChangeMainTheme(UToroRuntimeSettings::Get()->DefaultTheme.LoadSynchronous());
	});
}

void AWorldMusicManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	CleanOneShotTracks();
}
