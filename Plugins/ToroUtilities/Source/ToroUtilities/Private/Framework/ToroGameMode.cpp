// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroGameMode.h"
#include "Framework/ToroGameInstance.h"
#include "Framework/ToroWidgetManager.h"
#include "Framework/ToroPlayerController.h"
#include "Framework/ToroPlayerCharacter.h"
#include "Framework/ToroPlayerState.h"
#include "Framework/ToroGameState.h"

AToroGameMode::AToroGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);

	HUDClass = AToroWidgetManager::StaticClass();
	GameStateClass = AToroGameState::StaticClass();
	PlayerStateClass = AToroPlayerState::StaticClass();
	DefaultPawnClass = AToroPlayerCharacter::StaticClass();
	PlayerControllerClass = AToroPlayerController::StaticClass();
}

void AToroGameMode::BeginPlay()
{
	Super::BeginPlay();
	GameInstance = UToroGameInstance::Get(this);
	if (GameInstance) GameInstance->OnWorldBeginPlay(GetWorld());
}

void AToroGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GameInstance) GameInstance->OnWorldTick(DeltaTime);
}
