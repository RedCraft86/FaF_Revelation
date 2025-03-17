// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroCameraManager.h"

AToroCameraManager::AToroCameraManager()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
}

EToroValidPins AToroCameraManager::GetToroPlayerCameraManager(AToroCameraManager*& OutObject,
	const UObject* ContextObject, const TSubclassOf<AToroCameraManager>& Class, const int32 PlayerIndex)
{
	AToroCameraManager* Obj = Get<AToroCameraManager>(ContextObject, PlayerIndex);
	OutObject = IsValid(Obj) && (!Class || Obj->IsA(Class)) ? Obj : nullptr;
	return IsValid(OutObject) ? EToroValidPins::Valid : EToroValidPins::NotValid;
}

void AToroCameraManager::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->bIsCameraMoveableWhenPaused = true;
}

void AToroCameraManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (GetWorld()->IsPaused() && GetTickableWhenPaused())
	{
		UpdateCamera(DeltaSeconds);
	}
}
