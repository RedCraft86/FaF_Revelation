// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroPlayerController.h"
#include "Framework/ToroPlayerCharacter.h"
#include "Framework/ToroCameraManager.h"
#include "Blueprint/UserWidget.h"

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

void AToroPlayerController::OnAnyKeyEvent(FKey PressedKey)
{
	if (OnAnyKeyPressed.IsBound()) OnAnyKeyPressed.Broadcast(PressedKey);
}

void AToroPlayerController::OnWindowFocusChanged(bool bFocused)
{
	SetPause(!bFocused);
}

void AToroPlayerController::BeginPlay()
{
	Super::BeginPlay();
	FSlateApplication::Get().OnApplicationActivationStateChanged()
		.AddUObject(this, &AToroPlayerController::OnWindowFocusChanged);
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
