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

void AToroPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	InputModeData.ClearReferences();
	Super::EndPlay(EndPlayReason);
}

void AToroPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = GetEnhancedInputSubsystem())
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(UToroRuntimeSettings::Get()->DefaultInputMappings.LoadSynchronous(), 0);
	}
}
