// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroPlayerController.h"
#include "Framework/ToroCameraManager.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"

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
	if (InObject && PauseRequests.Contains(InObject))
	{
		PauseRequests.Remove(InObject);
		UpdatePauseState();
	}
}

void AToroPlayerController::ExitCinematic()
{
	CinematicActor = nullptr;
	SetCinematicMode(false, false, false, true, true);
}

void AToroPlayerController::EnterCinematic(AActor* InActor)
{
	CinematicActor = InActor;
	if (CinematicActor.IsValid())
	{
		SetCinematicMode(true, true, false, true, true);
	}
	else
	{
		ExitCinematic();
	}
}

// void AToroPlayerController::CheckValidity()
// {
// 	// Check pause requests
// 	if (!PauseRequests.IsEmpty())
// 	{
// 		bool bChanged = false;
// 		for (const TWeakObjectPtr<const UObject>& Element : PauseRequests)
// 		{
// 			if (!Element.IsValid())
// 			{
// 				bChanged = true;
// 				break;
// 			}
// 		}
//
// 		if (bChanged)
// 		{
// 			UpdatePauseState();
// 		}
// 	}
//
// 	// Check cinematic actor
// 	if (bCinematicMode && !CinematicActor.IsValid())
// 	{
// 		ExitCinematic();
// 	}
// }

void AToroPlayerController::UpdatePauseState()
{
	for (auto It = PauseRequests.CreateIterator(); It; ++It)
	{
		if (!It->IsValid())
		{
			It.RemoveCurrent();
		}
	}

	SetPause(bUserPaused || !PauseRequests.IsEmpty());
}

void AToroPlayerController::OnWindowFocusChanged(bool bFocused)
{
	OnGameFocusChanged.Broadcast(bFocused);

	if (const ALevelSequenceActor* Cinematic = Cast<ALevelSequenceActor>(GetCinematicActor()))
	{
		if (ULevelSequencePlayer* SequencePlayer = Cinematic->GetSequencePlayer())
		{
			bFocused ? SequencePlayer->Play() : SequencePlayer->Pause();
		}
	}

	if (bFocused)
	{
		RemovePauseRequest(this);
	}
	else /*if (UToroFrameworkSettings::Get()->IsPlayableMap(this))*/
	{
		AddPauseRequest(this);
	}
}

void AToroPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// GetWorldTimerManager().SetTimer(ValidityCheckTimer, this,
	// 	&AToroPlayerController::CheckValidity, 5.0f, true);
	
	FSlateApplication::Get().OnApplicationActivationStateChanged()
		.AddUObject(this, &AToroPlayerController::OnWindowFocusChanged);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = GetEnhancedInputSubsystem())
	{
		Subsystem->ClearAllMappings();
		//Subsystem->AddMappingContext(UToroFrameworkSettings::Get()->InputMappings.LoadSynchronous(), 0); // TODO
		SetInputConfig({EGameInputMode::GameOnly, false});
	}
}

void AToroPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CachedInputConfig.ClearReferences();
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
