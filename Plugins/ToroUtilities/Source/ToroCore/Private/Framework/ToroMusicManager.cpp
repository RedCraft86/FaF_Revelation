// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroMusicManager.h"
#include "Components/AudioComponent.h"

AToroMusicManager::AToroMusicManager()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;
	PrimaryActorTick.TickInterval = 1.0f;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);

	ThemeComponent = CreateDefaultSubobject<UAudioComponent>("ThemeComponent");
	ThemeComponent->SetupAttachment(SceneRoot);
	ThemeComponent->SetUISound(true);
	ThemeComponent->bCanPlayMultipleInstances = false;
	ThemeComponent->bAutoActivate = false;
	ThemeComponent->bIsMusic = true;
}

EToroValidPins AToroMusicManager::GetMusicManager(AToroMusicManager*& OutObject,
	const UObject* ContextObject, const TSubclassOf<AToroMusicManager>& Class)
{
	AToroMusicManager* Obj = Get<AToroMusicManager>(ContextObject);
	OutObject = IsValid(Obj) && (!Class || Obj->IsA(Class)) ? Obj : nullptr;
	return IsValid(OutObject) ? EToroValidPins::Valid : EToroValidPins::NotValid;
}
