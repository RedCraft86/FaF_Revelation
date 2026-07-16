// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "GamePlayerController.h"
#include "GamePlayerCharacter.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "Framework/ToroPlayerHUD.h"
#include "UserWidgets/GamePauseWidget.h"
#include "UserWidgets/GameplayWidget.h"
#include "Messaging/MessageManager.h"
#include "GameSettings.h"
#include "TimerManager.h"

AGamePlayerController::AGamePlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;

	MessageManager = CreateDefaultSubobject<UMessageManager>("MessageManager");
}

void AGamePlayerController::SetGamePaused(const bool bPaused)
{
	const AToroPlayerHUD* PlayerHUD = AToroPlayerHUD::Get(this);
	const UToroMasterWidget* MasterWidget = PlayerHUD ? PlayerHUD->GetMasterWidget() : nullptr;
	if (!MasterWidget || !PauseWidgetClass)
	{
		return;
	}

	if (bPaused)
	{
		MasterWidget->PushToStack(PauseWidgetClass);
	}
	else
	{
		MasterWidget->PopFromStack(PauseWidgetClass);
	}

	SetPause(bPaused);
}

void AGamePlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (UGameSettings::Get()->IsInMapType(this, EGameMapType::Gameplay))
	{
		const ULocalPlayer* LP = GetLocalPlayer();
		UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LP ? 
			LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>() : nullptr;
		if (InputSubsystem && InputMappings)
		{
			InputSubsystem->AddMappingContext(InputMappings, 0);
		}
	}
}

void AGamePlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGamePlayerController::SetCinematicMode(bool bInCinematicMode, bool bAffectsMovement, bool bAffectsTurning)
{
	Super::SetCinematicMode(bInCinematicMode, bAffectsMovement, bAffectsTurning);
	if (AGamePlayerCharacter* PlayerChar = AGamePlayerCharacter::Get<AGamePlayerCharacter>(this))
	{
		PlayerChar->OnCinematic(bInCinematicMode);
	}
}
