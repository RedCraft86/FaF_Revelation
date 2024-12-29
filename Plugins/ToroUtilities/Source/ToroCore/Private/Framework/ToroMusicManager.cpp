// Copyright (C) RedCraft86. All Rights Reserved.


#include "ToroMusicManager.h"

AToroMusicManager::AToroMusicManager()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);
}

EToroValidPins AToroMusicManager::GetToroMusicManager(AToroMusicManager*& OutObject,
	const UObject* WorldContextObject, const TSubclassOf<AToroMusicManager>& Class)
{
	AToroMusicManager* Obj = Cast<AToroMusicManager>(UGameplayStatics::GetGameState(WorldContextObject));
	OutObject = IsValid(Obj) && (!Class || Obj->IsA(Class)) ? Obj : nullptr;
	return IsValid(OutObject) ? EToroValidPins::Valid : EToroValidPins::NotValid;
}
