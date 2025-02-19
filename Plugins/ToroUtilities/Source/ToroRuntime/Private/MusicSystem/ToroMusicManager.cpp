// Copyright (C) RedCraft86. All Rights Reserved.

#include "MusicSystem/ToroMusicManager.h"
#include "ToroRuntimeSettings.h"
#include "EnhancedCodeFlow.h"

AToroMusicManager::AToroMusicManager()
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

EToroValidPins AToroMusicManager::GetMusicManager(AToroMusicManager*& OutObject,
	const UObject* WorldContextObject, const TSubclassOf<AToroMusicManager>& Class)
{
	AToroMusicManager* Obj = Cast<AToroMusicManager>(UGameplayStatics::GetGameState(WorldContextObject));
	OutObject = IsValid(Obj) && (!Class || Obj->IsA(Class)) ? Obj : nullptr;
	return IsValid(OutObject) ? EToroValidPins::Valid : EToroValidPins::NotValid;
}

bool AToroMusicManager::ChangeMainTheme(UMetaSoundSource* NewTheme)
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

	SetThemeIntensity(0.0f);
	SetThemeState(0);
	return true;
}

void AToroMusicManager::SetThemeIntensity(const float InIntensity) const
{
	GetSoundParamInterface()->SetFloatParameter(TEXT("Intensity"), InIntensity);
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
	return Cast<IAudioParameterControllerInterface>(MainThemeComponent);
}

void AToroMusicManager::BeginPlay()
{
	Super::BeginPlay();
	const UToroRuntimeSettings* Settings = UToroRuntimeSettings::Get();
	if (!Settings->IsOnGameplayMap(this))
	{
		FEnhancedCodeFlow::Delay(this, 1.0f, [this, Settings]()
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
