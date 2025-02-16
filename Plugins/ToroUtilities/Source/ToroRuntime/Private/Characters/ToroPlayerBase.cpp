// Copyright (C) RedCraft86. All Rights Reserved.

#include "Characters/ToroPlayerBase.h"
#include "Framework/ToroPlayerController.h"

UE_DEFINE_GAMEPLAY_TAG(Tag_Player, "Character.Player");

AToroPlayerBase::AToroPlayerBase()
{
	PrimaryActorTick.bCanEverTick = true;
	CharacterID = Tag_Player;
	
	Camera = CreateDefaultSubobject<UCineCameraComponent>(TEXT("Camera"));
	Camera->SetRelativeLocation({0.0f, 0.0f, 70.0f});
	Camera->SetupAttachment(GetMesh());

	VisionComponent->SetRelativeLocation(FVector::ZeroVector);
	VisionComponent->SetupAttachment(GetMesh());
}

EToroValidPins AToroPlayerBase::GetToroPlayerCharacter(AToroPlayerBase*& OutObject,
	const UObject* WorldContextObject, const TSubclassOf<AToroPlayerBase>& Class, const int32 PlayerIndex)
{
	AToroPlayerBase* Obj = Cast<AToroPlayerBase>(UGameplayStatics::GetPlayerCharacter(WorldContextObject, PlayerIndex));
	OutObject = IsValid(Obj) && (!Class || Obj->IsA(Class)) ? Obj : nullptr;
	return IsValid(OutObject) ? EToroValidPins::Valid : EToroValidPins::NotValid;
}

AToroPlayerController* AToroPlayerBase::GetPlayerController() const
{
	return GetController<AToroPlayerController>();
}

bool AToroPlayerBase::GetLookTarget_Implementation(FVector& Target)
{
	// TODO: Return a lock-on target's location
	return false;
}

void AToroPlayerBase::GetViewPoint_Implementation(FVector& Location, FVector& Forward, float& Angle)
{
	Location = Camera->GetComponentLocation();
	Forward = Camera->GetForwardVector();
	Angle = Camera->FieldOfView;
}

void AToroPlayerBase::BeginPlay()
{
	Super::BeginPlay();
}

void AToroPlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AToroPlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
