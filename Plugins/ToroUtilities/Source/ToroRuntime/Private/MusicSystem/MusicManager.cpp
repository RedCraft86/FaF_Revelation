// Copyright (C) RedCraft86. All Rights Reserved.

#include "MusicSystem/MusicManager.h"

AMusicManager::AMusicManager()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);
}

EToroValidPins AMusicManager::GetMusicManager(AMusicManager*& OutObject,
	const UObject* WorldContextObject, const TSubclassOf<AMusicManager>& Class)
{
	AMusicManager* Obj = Cast<AMusicManager>(UGameplayStatics::GetGameState(WorldContextObject));
	OutObject = IsValid(Obj) && (!Class || Obj->IsA(Class)) ? Obj : nullptr;
	return IsValid(OutObject) ? EToroValidPins::Valid : EToroValidPins::NotValid;
}
