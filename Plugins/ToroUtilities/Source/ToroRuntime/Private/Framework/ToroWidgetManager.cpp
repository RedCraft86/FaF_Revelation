// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroWidgetManager.h"

AToroWidgetManager::AToroWidgetManager()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;
}

EToroValidPins AToroWidgetManager::GetToroWidgetManager(AToroWidgetManager*& OutObject,
	const UObject* WorldContextObject, const TSubclassOf<AToroWidgetManager>& Class, const int32 PlayerIndex)
{
	AToroWidgetManager* Obj = Cast<AToroWidgetManager>(UGameplayStatics::GetPlayerState(WorldContextObject, PlayerIndex));
	OutObject = IsValid(Obj) && (!Class || Obj->IsA(Class)) ? Obj : nullptr;
	return IsValid(OutObject) ? EToroValidPins::Valid : EToroValidPins::NotValid;
}

void AToroWidgetManager::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PlayerController = GetPlayerController();
}
