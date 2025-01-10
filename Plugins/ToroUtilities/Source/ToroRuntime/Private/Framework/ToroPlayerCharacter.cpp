// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroPlayerCharacter.h"
#include "Framework/ToroPlayerController.h"

AToroPlayerCharacter::AToroPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

EToroValidPins AToroPlayerCharacter::GetToroPlayerCharacter(AToroPlayerCharacter*& OutObject,
	const UObject* WorldContextObject, const TSubclassOf<AToroPlayerCharacter>& Class, const int32 PlayerIndex)
{
	AToroPlayerCharacter* Obj = Cast<AToroPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(WorldContextObject, PlayerIndex));
	OutObject = IsValid(Obj) && (!Class || Obj->IsA(Class)) ? Obj : nullptr;
	return IsValid(OutObject) ? EToroValidPins::Valid : EToroValidPins::NotValid;
}

AToroPlayerController* AToroPlayerCharacter::GetPlayerController() const
{
	return GetController<AToroPlayerController>();
}

void AToroPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AToroPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AToroPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
