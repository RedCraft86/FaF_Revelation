// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroPlayerController.h"
#include "Framework/ToroPlayerCameraManager.h"
#include "Framework/ToroWidgetManager.h"
#include "EnhancedInputSubsystems.h"
#include "ToroRuntimeSettings.h"

AToroPlayerController::AToroPlayerController() : bGamePaused(false)
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);
	
	PlayerCameraManagerClass = AToroPlayerCameraManager::StaticClass();
}

EToroValidPins AToroPlayerController::GetToroPlayerController(AToroPlayerController*& OutObject,
	const UObject* WorldContextObject, const TSubclassOf<AToroPlayerController>& Class, const int32 PlayerIndex)
{
	AToroPlayerController* Obj = Cast<AToroPlayerController>(UGameplayStatics::GetPlayerController(WorldContextObject, PlayerIndex));
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

void AToroPlayerController::SetPauseState(const bool bInPaused)
{
	if (IsPaused() != bInPaused)
	{
		SetPause(bInPaused);
		bGamePaused = bInPaused;
		if (UPauseWidgetBase* Widget = GetPauseWidget())
		{
			bInPaused ? Widget->ActivateWidget() : Widget->DeactivateWidget();
		}
	}
}

void AToroPlayerController::OnAnyKeyEvent(FKey PressedKey)
{
	if (OnAnyKeyPressed.IsBound()) OnAnyKeyPressed.Broadcast(PressedKey);
}

void AToroPlayerController::OnWindowFocusChanged(bool bFocused)
{
	if (bFocused)
	{
		if (!bGamePaused && UToroRuntimeSettings::Get()->IsOnGameplayMap(this)) SetPause(false);
		if (const UInfoWidgetBase* Widget = GetInfoWidget())
		{
			Widget->MarkUnfocused(false);
		}

		UKismetSystemLibrary::ExecuteConsoleCommand(this, FString::Printf(
			TEXT("t.MaxFPS %f"), UToroUserSettings::Get()->GetFrameRateLimit()));

		if (const AToroPlayerBase* Player = GetPawn<AToroPlayerBase>())
		{
			if (const ALevelSequenceActor* Cinematic = Cast<ALevelSequenceActor>(Player->GetCinematicActor()))
			{
				Cinematic->GetSequencePlayer()->Play();
			}
		}
	}
	else
	{
		if (const AToroPlayerBase* Player = GetPawn<AToroPlayerBase>())
		{
			if (const ALevelSequenceActor* Cinematic = Cast<ALevelSequenceActor>(Player->GetCinematicActor()))
			{
				Cinematic->GetSequencePlayer()->Pause();
			}
		}

		if (!bGamePaused && UToroRuntimeSettings::Get()->IsOnGameplayMap(this)) SetPause(true);
		if (const UInfoWidgetBase* Widget = GetInfoWidget())
		{
			Widget->MarkUnfocused(true);
		}

		UKismetSystemLibrary::ExecuteConsoleCommand(this, TEXT("t.MaxFPS 5"));
	}
}

UInfoWidgetBase* AToroPlayerController::GetInfoWidget()
{
	if (InfoWidget) return InfoWidget;
	if (AToroWidgetManager* Manager = AToroWidgetManager::Get(this))
	{
		InfoWidget = Manager->FindWidget<UInfoWidgetBase>();
	}

	return InfoWidget;
}

UPauseWidgetBase* AToroPlayerController::GetPauseWidget()
{
	if (PauseWidget) return PauseWidget;
	if (AToroWidgetManager* Manager = AToroWidgetManager::Get(this))
	{
		PauseWidget = Manager->FindWidget<UPauseWidgetBase>();
	}

	return PauseWidget;
}

UEnhancedInputLocalPlayerSubsystem* AToroPlayerController::GetEnhancedInputSubsystem() const
{
	if (const ULocalPlayer* LocPlayer = GetLocalPlayer())
	{
		return LocPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	}
	
	return nullptr;
}

void AToroPlayerController::BeginPlay()
{
	Super::BeginPlay();
	FSlateApplication::Get().OnApplicationActivationStateChanged().AddUObject(this, &ThisClass::OnWindowFocusChanged);
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = GetEnhancedInputSubsystem())
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(UToroRuntimeSettings::Get()->DefaultInputMappings.LoadSynchronous(), 0);
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
