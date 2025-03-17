// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroPlayerController.h"
#include "ToroCameraManager.h"

AToroPlayerController::AToroPlayerController()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);

	PlayerCameraManagerClass = AToroCameraManager::StaticClass();
}

EToroValidPins AToroPlayerController::GetToroPlayerController(AToroPlayerController*& OutObject,
	const UObject* ContextObject, const TSubclassOf<AToroPlayerController>& Class, const int32 PlayerIndex)
{
	AToroPlayerController* Obj = Get<AToroPlayerController>(ContextObject, PlayerIndex);
	OutObject = IsValid(Obj) && (!Class || Obj->IsA(Class)) ? Obj : nullptr;
	return IsValid(OutObject) ? EToroValidPins::Valid : EToroValidPins::NotValid;
}
