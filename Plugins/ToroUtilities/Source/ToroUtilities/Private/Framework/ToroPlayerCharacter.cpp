// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroPlayerCharacter.h"

AToroPlayerCharacter::AToroPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
	AutoReceiveInput = EAutoReceiveInput::Player0;
}

EToroValidPins AToroPlayerCharacter::GetToroPlayerCharacter(AToroPlayerCharacter*& OutObject,
	const UObject* ContextObject, const TSubclassOf<AToroPlayerCharacter>& Class, const int32 PlayerIndex)
{
	AToroPlayerCharacter* Obj = Get<AToroPlayerCharacter>(ContextObject, PlayerIndex);
	OutObject = IsValid(Obj) && (!Class || Obj->IsA(Class)) ? Obj : nullptr;
	return IsValid(OutObject) ? EToroValidPins::Valid : EToroValidPins::NotValid;
}

void AToroPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AToroPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
