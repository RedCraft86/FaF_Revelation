// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroPlayerController.h"
#include "Framework/ToroPlayerCharacter.h"
#include "Framework/ToroCameraManager.h"
#include "EnhancedInputSubsystems.h"
#include "ToroSettings.h"

AToroPlayerController::AToroPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);

	PlayerCameraManagerClass = AToroCameraManager::StaticClass();
}

void AToroPlayerController::SetInputModeData(const FGameInputModeData& InputMode)
{
	InputModeData = InputMode;
	SetShowMouseCursor(InputModeData.ShouldShowMouse());
	SetInputMode(InputModeData.GetInputMode());
	FlushPressedKeys();
}

void AToroPlayerController::AddPauseRequest(const UObject* InObject)
{
	PauseRequests.Remove(nullptr);
	if (InObject)
	{
		PauseRequests.Add(InObject);
		UpdatePauseState();
	}
}

void AToroPlayerController::RemovePauseRequest(const UObject* InObject)
{
	PauseRequests.Remove(nullptr);
	if (InObject)
	{
		PauseRequests.Remove(InObject);
		UpdatePauseState();
	}
}

void AToroPlayerController::SetGamePaused(const bool bInPaused)
{
	if (IsPaused() != bInPaused)
	{
		bGamePaused = bInPaused;
		UpdatePauseState();
		OnGamePaused.Broadcast(bGamePaused);
	}
}

void AToroPlayerController::EnterCinematic(AActor* InActor)
{
	if (!CinematicActor)
	{
		CinematicActor = InActor;
		SetCinematicMode(true, true, false, true, true);
		if (AToroPlayerCharacter* PlayerChar = Cast<AToroPlayerCharacter>(GetPawn()))
		{
			PlayerChar->AddLockFlag(Tag_PlayerLock_Cinematic.GetTag());
		}
	}
}

void AToroPlayerController::ExitCinematic()
{
	if (CinematicActor)
	{
		CinematicActor = nullptr;
		SetCinematicMode(false, false, false, true, true);
		if (AToroPlayerCharacter* PlayerChar = Cast<AToroPlayerCharacter>(GetPawn()))
		{
			PlayerChar->ClearLockFlag(Tag_PlayerLock_Cinematic.GetTag());
		}
	}
}

void AToroPlayerController::UpdatePauseState()
{
	PauseRequests.Remove(nullptr);
	SetPause(bGamePaused || !PauseRequests.IsEmpty());
}

void AToroPlayerController::OnAnyKeyEvent(FKey PressedKey)
{
	if (OnAnyKeyPressed.IsBound()) OnAnyKeyPressed.Broadcast(PressedKey);
}

void AToroPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = GetEnhancedInputSubsystem())
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(UToroSettings::Get()->DefaultInputMappings.LoadSynchronous(), 0);
	}
}

void AToroPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	FInputKeyBinding& AnyKeyBinding = InputComponent->BindKey(EKeys::AnyKey,
		IE_Pressed, this, &AToroPlayerController::OnAnyKeyEvent);
	AnyKeyBinding.bConsumeInput = false; AnyKeyBinding.bExecuteWhenPaused = true;
}

void AToroPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	InputModeData.ClearReferences();
	Super::EndPlay(EndPlayReason);
}

bool AToroPlayerController::SetPause(bool bPause, FCanUnpause CanUnpauseDelegate)
{
	if (IsPaused() != bPause)
	{
		// if (const AToroMusicManager* MusicManager = AToroMusicManager::Get(this))
		// {
		// 	MusicManager->SetDipAudio(bPause); TODO
		// }
	}
	return Super::SetPause(bPause, CanUnpauseDelegate);
}

UEnhancedInputLocalPlayerSubsystem* AToroPlayerController::GetEnhancedInputSubsystem() const
{
	if (const ULocalPlayer* LocPlayer = GetLocalPlayer())
	{
		return LocPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	}
	return nullptr;
}
