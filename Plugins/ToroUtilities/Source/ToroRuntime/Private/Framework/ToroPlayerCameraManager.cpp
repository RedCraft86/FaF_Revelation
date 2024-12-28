// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroPlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"

AToroPlayerCameraManager::AToroPlayerCameraManager()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

EToroValidPins AToroPlayerCameraManager::GetToroPlayerCameraManager(AToroPlayerCameraManager*& OutObject,
	const UObject* WorldContextObject, const int32 PlayerIndex, const TSubclassOf<AToroPlayerCameraManager>& Class)
{
	AToroPlayerCameraManager* Obj = Cast<AToroPlayerCameraManager>(UGameplayStatics::GetPlayerCameraManager(WorldContextObject, PlayerIndex));
	OutObject = IsValid(Obj) && (!Class || Obj->IsA(Class)) ? Obj : nullptr;
	return IsValid(OutObject) ? EToroValidPins::Valid : EToroValidPins::NotValid;
}

template <typename T>
T* AToroPlayerCameraManager::Get(const UObject* WorldContextObject, const int32 PlayerIndex)
{
	return Cast<T>(UGameplayStatics::GetPlayerCameraManager(WorldContextObject, PlayerIndex));
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
