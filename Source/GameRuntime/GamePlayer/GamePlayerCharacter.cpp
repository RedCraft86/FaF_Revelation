// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "GamePlayerCharacter.h"
#include "Interaction/InteractionManager.h"

AGamePlayerCharacter::AGamePlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;

	Interaction = CreateDefaultSubobject<UInteractionManager>(TEXT("Interaction"));
}

void AGamePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AGamePlayerCharacter::SlowTick()
{
	Super::SlowTick();
}

void AGamePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGamePlayerCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AGamePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AGamePlayerCharacter::InputBinding_Pause(const FInputActionValue& InValue)
{
}

void AGamePlayerCharacter::InputBinding_Back(const FInputActionValue& InValue)
{
}

void AGamePlayerCharacter::InputBinding_Turn(const FInputActionValue& InValue)
{
}

void AGamePlayerCharacter::InputBinding_Move(const FInputActionValue& InValue)
{
}

void AGamePlayerCharacter::InputBinding_Run(const FInputActionValue& InValue)
{
}

void AGamePlayerCharacter::InputBinding_Crouch(const FInputActionValue& InValue)
{
}

void AGamePlayerCharacter::InputBinding_Lean(const FInputActionValue& InValue)
{
}

void AGamePlayerCharacter::InputBinding_ToggleQuests(const FInputActionValue& InValue)
{
}

void AGamePlayerCharacter::InputBinding_Interact(const FInputActionValue& InValue)
{
}

void AGamePlayerCharacter::InputBinding_Equipment(const FInputActionValue& InValue)
{
}
