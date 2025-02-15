// Copyright (C) RedCraft86. All Rights Reserved.

#include "VisionCone/VisionConeComponent.h"

UVisionConeComponent::UVisionConeComponent() : Distance(2000.0f), ViewAngle(50.0f), PeripheralAngle(10.0f)
{
	PrimaryComponentTick.bCanEverTick = false;
	TraceMethod = CreateDefaultSubobject<UActorTrace_Origin>(TEXT("TraceMethod"));
}

bool UVisionConeComponent::GetTraceToActor(const AActor* InActor) const
{
	if (!InActor) return false;
	return TraceMethod ? TraceMethod->TraceActor(this, InActor) : false;
}

float UVisionConeComponent::GetAngleToActor(const AActor* InActor) const
{
	if (!InActor) return -1.0f;
	const FVector DotA = InActor->GetActorLocation() - GetComponentLocation(), DotB = GetForwardVector();
	return 180.0f / UE_PI * FMath::Acos(FVector::DotProduct(DotA.GetSafeNormal(), DotB.GetSafeNormal()));
}

bool UVisionConeComponent::IsActorInRange(const AActor* InActor) const
{
	if (!InActor) return false;
	return FVector::Dist(GetComponentLocation(), InActor->GetActorLocation()) <= FMath::Max(Distance, 10.0f);
}

EVisionState UVisionConeComponent::GetActorVisionState(const AActor* InActor) const
{
	if (!IsActorInRange(InActor))
		return EVisionState::None;
	
	const float Angle = GetAngleToActor(InActor);
	if (Angle >= 0.0f && Angle <= GetPeripheralAngle() && GetTraceToActor(InActor))
	{
		return Angle <= GetViewAngle() ? EVisionState::Sighted : EVisionState::Peripheral;
	}
	
	return EVisionState::None;
}
