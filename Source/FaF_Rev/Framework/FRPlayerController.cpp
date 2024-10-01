// Copyright (C) RedCraft86. All Rights Reserved.

#include "FRPlayerController.h"
#include "LevelSequencePlayer.h"
#include "Blueprint/UserWidget.h"
#include "EnhancedInputSubsystems.h"
#include "GameSection/GameSectionManager.h"
#include "Menus/PauseWidget.h"
#include "FRGameMode.h"

AFRPlayerController::AFRPlayerController()
	: MappingContext(nullptr), UnfocusedWidget(nullptr), PauseWidget(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
}

UEnhancedInputLocalPlayerSubsystem* AFRPlayerController::GetInputSubsystem() const
{
	return GetLocalPlayer() ? GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>() : nullptr;
}

void AFRPlayerController::SetPauseState(const bool bInPaused)
{
	if (IsPaused() != bInPaused)
	{
		SetPause(bInPaused);
		if (!PauseWidget)
		{
			PauseWidget = FRGameMode(this)->GetWidget<UPauseWidgetBase>();
		}

		bGamePaused = bInPaused;
		if (bInPaused)
		{
			PauseWidget->AddWidget(nullptr);
		}
		else
		{
			PauseWidget->RemoveWidget(nullptr);
		}
	}
}

void AFRPlayerController::OnWindowFocusChanged(bool bFocused)
{
	if (!IsValid(UnfocusedWidget))
	{
		UnfocusedWidget = CreateWidget<UUserWidget>(GetWorld(), UnfocusedWidgetClass);
	}

	if (!FRSettings->IsGameplayMap(this) || UGameSectionManager::Get(this)->IsLoading())
	{
#if !WITH_EDITOR
		UKismetSystemLibrary::ExecuteConsoleCommand(this, FString::Printf(TEXT("t.MaxFPS %f"),
				bFocused ? UGameSettings::Get()->GetFrameRateLimit() : 4.0f));
#endif
		return;
	}
	
#if !WITH_EDITOR
	UKismetSystemLibrary::ExecuteConsoleCommand(this, FString::Printf(TEXT("t.MaxFPS %f"),
		UGameSettings::Get()->GetFrameRateLimit()));
#endif

	if (bGamePaused) return;
	if (bFocused)
	{
		SetPause(false);
		if (UnfocusedWidget->IsInViewport())
		{
			UnfocusedWidget->RemoveFromParent();
		}
	}
	else if (!IsPaused() || !UnfocusedWidget->IsInViewport())
	{
		UnfocusedWidget->AddToViewport(100);
		SetPause(true);
	}

	if (const AFRPlayerBase* PlayerChar = Cast<AFRPlayerBase>(GetPawn()))
	{
		if (PlayerChar->GetActiveCutscene())
		{
			UE_PRINT(1.0f, Green, TEXT("Pausing: %s"), *PlayerChar->GetActiveCutscene()->GetSequenceName())
			bFocused ? PlayerChar->GetActiveCutscene()->Play()
				: PlayerChar->GetActiveCutscene()->Pause();
		}
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
// ReSharper disable once CppPassValueParameterByConstReference
void AFRPlayerController::OnAnyKeyEvent(FKey PressedKey)
{
	if (OnAnyKeyPressed.IsBound()) OnAnyKeyPressed.Broadcast(PressedKey);
}

void AFRPlayerController::BeginPlay()
{
	Super::BeginPlay();
	PlayerCameraManager->SetTickableWhenPaused(true);
	FSlateApplication::Get().OnApplicationActivationStateChanged().AddUObject(this, &AFRPlayerController::OnWindowFocusChanged);
	GetInputSubsystem()->AddMappingContext(MappingContext, 0);
}

void AFRPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	FInputKeyBinding& AnyKeyBinding = InputComponent->BindKey(EKeys::AnyKey, IE_Pressed, this, &AFRPlayerController::OnAnyKeyEvent);
	AnyKeyBinding.bConsumeInput = false; AnyKeyBinding.bExecuteWhenPaused = true;
}
