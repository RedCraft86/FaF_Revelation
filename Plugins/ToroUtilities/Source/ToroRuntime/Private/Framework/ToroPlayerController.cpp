﻿// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroPlayerController.h"
#include "Framework/ToroCameraManager.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "ToroRuntime.h"

AToroPlayerController::AToroPlayerController(): bUserPaused(false)
{
	PrimaryActorTick.bCanEverTick = true;
	PlayerCameraManagerClass = AToroCameraManager::StaticClass();

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);
}

void AToroPlayerController::SetInputConfig(const FGameInputConfig& InputConfig)
{
	CachedInputConfig = InputConfig;
	SetShowMouseCursor(CachedInputConfig.ShouldShowMouse());
	SetInputMode(CachedInputConfig.GetInputMode());
	FlushPressedKeys();
}

void AToroPlayerController::SetUserPause(bool bShouldPause)
{
	if (bUserPaused != bShouldPause)
	{
		bUserPaused = bShouldPause;
		OnUserPaused.Broadcast(bUserPaused);
		UpdatePauseState();
	}
}

void AToroPlayerController::AddPauseRequest(const UObject* InObject)
{
	if (InObject && !PauseRequests.Contains(InObject))
	{
		PauseRequests.Add(InObject);
		UpdatePauseState();
	}
}

void AToroPlayerController::RemovePauseRequest(const UObject* InObject)
{
	if (InObject && PauseRequests.Remove(InObject) > 0)
	{
		UpdatePauseState();
	}
}

void AToroPlayerController::ExitCinematic()
{
	CinematicActor = nullptr;
	SetCinematicMode(false, false, false, true, true);
}

void AToroPlayerController::EnterCinematic(AActor* InInstigator)
{
	CinematicActor = InInstigator;
	if (CinematicActor.IsValid())
	{
		SetCinematicMode(true, true, false, true, true);
	}
	else
	{
		ExitCinematic();
	}
}

void AToroPlayerController::UpdatePauseState()
{
	for (auto It = PauseRequests.CreateIterator(); It; ++It)
	{
		if (!It->IsValid())
		{
			It.RemoveCurrent();
		}
	}

	const bool bTargetPaused = bUserPaused || !PauseRequests.IsEmpty();

	if (const ALevelSequenceActor* Cinematic = Cast<ALevelSequenceActor>(GetCinematicActor()))
	{
		if (ULevelSequencePlayer* SequencePlayer = Cinematic->GetSequencePlayer())
		{
			bTargetPaused ? SequencePlayer->Pause() : SequencePlayer->Play();
		}
	}

	SetPause(bTargetPaused);
}

void AToroPlayerController::OnWindowFocusChanged(bool bFocused)
{
	OnGameFocusChanged.Broadcast(bFocused);

	if (bFocused)
	{
		RemovePauseRequest(this);
	}
	else if (!UToroSettings::Get()->IsOnMap(this, EToroMapType::MainMenu))
	{
		AddPauseRequest(this);
	}
}

void AToroPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FSlateApplication::Get().OnApplicationActivationStateChanged()
		.AddUObject(this, &AToroPlayerController::OnWindowFocusChanged);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = GetEnhancedInputSubsystem())
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(UToroSettings::Get()->InputMappings.LoadSynchronous(), 0);
		SetInputConfig({EGameInputMode::GameOnly, false});
	}
}

void AToroPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CachedInputConfig.ClearAndReset();
	Super::EndPlay(EndPlayReason);
}

UEnhancedInputLocalPlayerSubsystem* AToroPlayerController::GetEnhancedInputSubsystem() const
{
	if (const ULocalPlayer* LocPlayer = GetLocalPlayer())
	{
		return LocPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	}
	return nullptr;
}
