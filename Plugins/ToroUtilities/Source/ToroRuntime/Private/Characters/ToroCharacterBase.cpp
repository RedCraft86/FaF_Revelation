// Copyright (C) RedCraft86. All Rights Reserved.

#include "Characters/ToroCharacterBase.h"

UE_DEFINE_GAMEPLAY_TAG(Tag_Character, "Character");
UE_DEFINE_GAMEPLAY_TAG(Tag_Friendly, "Character.Friendly");
UE_DEFINE_GAMEPLAY_TAG(Tag_Enemy, "Character.Enemy");

AToroCharacterBase::AToroCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	VisionComponent = CreateDefaultSubobject<UVisionComponent>(TEXT("VisionComponent"));
	VisionComponent->SetRelativeLocation({0.0f, 0.0f, 70.0f});
	VisionComponent->SetupAttachment(GetMesh());
}

void AToroCharacterBase::GetViewPoint_Implementation(FVector& Location, FVector& Forward, float& Angle)
{
	Location = VisionComponent->GetComponentLocation();
	Forward = VisionComponent->GetForwardVector();
	Angle = VisionComponent->ViewAngle;
}

void AToroCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AToroCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AToroCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
