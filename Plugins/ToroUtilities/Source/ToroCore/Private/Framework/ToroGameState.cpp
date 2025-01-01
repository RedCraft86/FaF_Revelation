// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroGameState.h"

AToroGameState::AToroGameState()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);
}

EToroValidPins AToroGameState::GetToroGameState(AToroGameState*& OutObject,
	const UObject* WorldContextObject, const TSubclassOf<AToroGameState>& Class)
{
	AToroGameState* Obj = Cast<AToroGameState>(UGameplayStatics::GetGameState(WorldContextObject));
	OutObject = IsValid(Obj) && (!Class || Obj->IsA(Class)) ? Obj : nullptr;
	return IsValid(OutObject) ? EToroValidPins::Valid : EToroValidPins::NotValid;
}
