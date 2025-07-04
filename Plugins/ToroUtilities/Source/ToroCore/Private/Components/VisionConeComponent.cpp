// Copyright (C) RedCraft86. All Rights Reserved.

#include "Components/VisionConeComponent.h"

UVisionConeComponent::UVisionConeComponent(): Distance(2000.0f), OuterAngle(50.0f), InnerAngle(10.0f)
{
	PrimaryComponentTick.bCanEverTick = false;
	TraceMethod = CreateDefaultSubobject<UVisionTrace_Origin>("TraceMethod");
}

bool UVisionConeComponent::GetTraceToActor(const AActor* InActor) const
{
	return InActor && (!TraceMethod || TraceMethod->TraceActor(this, InActor));
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
	if (IsActorInRange(InActor) && GetTraceToActor(InActor))
	{
		if (const float Angle = GetAngleToActor(InActor); Angle >= 0.0f && Angle <= GetOuterAngle())
		{
			return Angle <= GetInnerAngle() ? EVisionState::Sighted : EVisionState::Peripheral;
		}
	}
	
	return EVisionState::None;
}
