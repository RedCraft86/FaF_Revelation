﻿// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroGameMode.h"
#include "ToroMusicManager.h"
#include "ToroWidgetManager.h"
#include "ToroPlayerCharacter.h"
#include "ToroPlayerController.h"
#include "Kismet/GameplayStatics.h"

AToroGameMode::AToroGameMode()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);

	GameStateClass = AToroMusicManager::StaticClass();
	PlayerControllerClass = AToroPlayerController::StaticClass();
	PlayerStateClass = AToroWidgetManager::StaticClass();
	DefaultPawnClass = AToroPlayerCharacter::StaticClass();
}

EToroValidPins AToroGameMode::GetToroGameMode(AToroGameMode*& OutObject,
	const UObject* WorldContextObject, const TSubclassOf<AToroGameMode>& Class)
{
	AToroGameMode* Obj = Cast<AToroGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	OutObject = IsValid(Obj) && (!Class || Obj->IsA(Class)) ? Obj : nullptr;
	return IsValid(OutObject) ? EToroValidPins::Valid : EToroValidPins::NotValid;
}

template <typename T>
T* AToroGameMode::Get(const UObject* WorldContextObject)
{
	return Cast<T>(UGameplayStatics::GetGameMode(WorldContextObject));
}

void AToroGameMode::BeginPlay()
{
	Super::BeginPlay();
	GameInstance = UToroGameInstance::Get(this);
	if (GameInstance) GameInstance->OnWorldBeginPlay(GetWorld());
}

void AToroGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (GameInstance) GameInstance->OnWorldTick(DeltaSeconds);
}
