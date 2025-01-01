// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroPlayerCameraManager.h"

AToroPlayerCameraManager::AToroPlayerCameraManager()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

EToroValidPins AToroPlayerCameraManager::GetToroPlayerCameraManager(AToroPlayerCameraManager*& OutObject,
	const UObject* WorldContextObject, const TSubclassOf<AToroPlayerCameraManager>& Class, const int32 PlayerIndex)
{
	AToroPlayerCameraManager* Obj = Cast<AToroPlayerCameraManager>(UGameplayStatics::GetPlayerCameraManager(WorldContextObject, PlayerIndex));
	OutObject = IsValid(Obj) && (!Class || Obj->IsA(Class)) ? Obj : nullptr;
	return IsValid(OutObject) ? EToroValidPins::Valid : EToroValidPins::NotValid;
}

void AToroPlayerCameraManager::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->bIsCameraMoveableWhenPaused = true;
}

void AToroPlayerCameraManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (GetWorld()->IsPaused() && GetTickableWhenPaused())
	{
		UpdateCamera(DeltaSeconds);
	}
}
