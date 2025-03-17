// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroGameMode.h"
#include "ToroMusicManager.h"
#include "ToroWidgetManager.h"
#include "ToroPlayerCharacter.h"
#include "ToroPlayerController.h"

AToroGameMode::AToroGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);

	GameStateClass = AToroMusicManager::StaticClass();
	PlayerStateClass = AToroWidgetManager::StaticClass();
	DefaultPawnClass = AToroPlayerCharacter::StaticClass();
	PlayerControllerClass = AToroPlayerController::StaticClass();
}

EToroValidPins AToroGameMode::GetToroGameMode(AToroGameMode*& OutObject,
	const UObject* ContextObject, const TSubclassOf<AToroGameMode>& Class)
{
	AToroGameMode* Obj = Get<AToroGameMode>(ContextObject);
	OutObject = IsValid(Obj) && (!Class || Obj->IsA(Class)) ? Obj : nullptr;
	return IsValid(OutObject) ? EToroValidPins::Valid : EToroValidPins::NotValid;
}
