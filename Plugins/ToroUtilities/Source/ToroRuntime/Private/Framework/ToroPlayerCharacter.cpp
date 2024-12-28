// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

AToroPlayerCharacter::AToroPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

EToroValidPins AToroPlayerCharacter::GetToroPlayerCharacter(AToroPlayerCharacter*& OutObject,
	const UObject* WorldContextObject, const int32 PlayerIndex, const TSubclassOf<AToroPlayerCharacter>& Class)
{
	AToroPlayerCharacter* Obj = Cast<AToroPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(WorldContextObject, PlayerIndex));
	OutObject = IsValid(Obj) && (!Class || Obj->IsA(Class)) ? Obj : nullptr;
	return IsValid(OutObject) ? EToroValidPins::Valid : EToroValidPins::NotValid;
}

template <typename T>
T* AToroPlayerCharacter::Get(const UObject* WorldContextObject, const int32 PlayerIndex)
{
	return Cast<T>(UGameplayStatics::GetPlayerCharacter(WorldContextObject, PlayerIndex));
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
