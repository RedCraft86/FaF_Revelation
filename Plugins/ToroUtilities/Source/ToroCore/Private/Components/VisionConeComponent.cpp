// Copyright (C) RedCraft86. All Rights Reserved.

#include "Components/VisionConeComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "EngineUtils.h"

UVisionConeComponent::UVisionConeComponent(): TraceChannel(ECC_Visibility), bDetectCharacters(false)
{
	PrimaryComponentTick.TickInterval = 0.1f;
}

EVisionState UVisionConeComponent::GetActorVisionState(const AActor* InActor) const
{
	if (!IsActorInRange(InActor) || !GetTraceToActor(InActor))
	{
		return EVisionState::None;
	}

	const float Angle = GetAngleToActor(InActor);
	if (Angle >= 0.0f && Angle <= GetOuterAngle())
	{
		return Angle <= GetInnerAngle() ? EVisionState::Sighted : EVisionState::Peripheral;
	}
	
	return EVisionState::None;
}

bool UVisionConeComponent::GetTraceToActor(const AActor* InActor) const
{
	if (!InActor) return false;

	FVector Target = InActor->GetActorLocation();
	if (const ACharacter* Char = Cast<ACharacter>(InActor))
	{
		Target.Z += Char->GetCapsuleComponent()->GetScaledCapsuleHalfHeight_WithoutHemisphere();
	}

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(InActor);
	Params.AddIgnoredActor(GetOwner());
	return !GetWorld()->LineTraceSingleByChannel(Hit, GetComponentLocation(), Target, TraceChannel, Params);
}

float UVisionConeComponent::GetAngleToActor(const AActor* InActor) const
{
	if (!InActor) return -1.0f;
	const FVector DotA = InActor->GetActorLocation() - GetComponentLocation(), DotB = GetForwardVector();
	return 180.0f / UE_PI * FMath::Acos(FVector::DotProduct(DotA.GetSafeNormal(), DotB.GetSafeNormal()));
}

bool UVisionConeComponent::IsActorInRange(const AActor* InActor) const
{
	return InActor && FVector::Dist(GetComponentLocation(), InActor->GetActorLocation()) <= VisionCone.Distance;
}

void UVisionConeComponent::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(bDetectCharacters);
}

void UVisionConeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	for (auto It = DetectionCache.CreateIterator(); It; ++It)
	{
		if (!It->Key.IsValid()) It.RemoveCurrent();
	}

	if (!bDetectCharacters) return;
	for (const ACharacter* Char : TActorRange<ACharacter>(GetWorld()))
	{
		EVisionState State = GetActorVisionState(Char);
		if (DetectionCache.FindOrAdd(Char, EVisionState::None) != State)
		{
			DetectionCache.Add(Char, State);
			OnCharacterState.Broadcast(Char, State);
			OnCharacterStateBP.Broadcast(Char, State);
		}
	}
}
