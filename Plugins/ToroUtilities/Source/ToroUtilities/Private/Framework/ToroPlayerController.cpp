// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroPlayerController.h"
#include "Framework/ToroPlayerCharacter.h"
#include "Framework/ToroCameraManager.h"
#include "Framework/ToroMusicManager.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "ToroSettings.h"

void FGameInputModeData::ClearReferences()
{
	UI_Only = FInputModeUIOnly();
	GameAndUI = FInputModeGameAndUI();
	FocusWidget = nullptr;
}

const FInputModeDataBase& FGameInputModeData::GetInputMode()
{
	switch (InputMode)
	{
	case EGameInputMode::UI_Only:
		UI_Only.SetLockMouseToViewportBehavior(MouseLock);
		if (FocusWidget) UI_Only.SetWidgetToFocus(FocusWidget->TakeWidget());
		return UI_Only;
		
	case EGameInputMode::GameAndUI:
		GameAndUI.SetLockMouseToViewportBehavior(MouseLock);
		GameAndUI.SetHideCursorDuringCapture(bHideCursorOnCapture);
		if (FocusWidget) GameAndUI.SetWidgetToFocus(FocusWidget->TakeWidget());
		return GameAndUI;

	default: return GameOnly;
	}
}

AToroPlayerController::AToroPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);

	PlayerCameraManagerClass = AToroCameraManager::StaticClass();
}

EToroValidPins AToroPlayerController::GetToroPlayerController(AToroPlayerController*& OutObject,
	const UObject* ContextObject, const TSubclassOf<AToroPlayerController>& Class, const int32 PlayerIndex)
{
	AToroPlayerController* Obj = Get<AToroPlayerController>(ContextObject, PlayerIndex);
	OutObject = IsValid(Obj) && (!Class || Obj->IsA(Class)) ? Obj : nullptr;
	return IsValid(OutObject) ? EToroValidPins::Valid : EToroValidPins::NotValid;
}

void AToroPlayerController::SetGameInputMode(const EGameInputMode InputMode, const bool bMouseCursor,
	const EMouseLockMode MouseLock, const bool bHideCursorOnCapture, UUserWidget* FocusWidget)
{
	SetInputModeData({InputMode, bMouseCursor, MouseLock, bHideCursorOnCapture, FocusWidget});
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
		IE_Pressed, this, &ThisClass::OnAnyKeyEvent);
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
		if (const AToroMusicManager* MusicManager = AToroMusicManager::Get(this))
		{
			MusicManager->SetDipAudio(bPause);
		}
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
