// Copyright (C) RedCraft86. All Rights Reserved.

#include "Components/VisionConeComponent.h"
#include "Libraries/GTRuntimeLibrary.h"

UVisionConeComponent::UVisionConeComponent()
	: TraceChannel(ECC_Visibility), MaxDistance(2000.0f), BaseAngle(50.0f), PeripheralAngle(20.0f)
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UVisionConeComponent::GetTraceToActor(const AActor* InActor, const bool bCollidingOnly) const
{
	if (!InActor) return false;
	const FVector TraceStart = GetComponentLocation();

	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(InActor);
	TraceParams.AddIgnoredActor(GetOwner());
		
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart,
		InActor->GetActorLocation(), TraceChannel, TraceParams);
	if (!HitResult.bBlockingHit) return true;

	FVector Origin, BoxExtent;
	TArray<FVector> Vertices = UGTRuntimeLibrary::GetBoundingBoxVertices(InActor,
		bCollidingOnly, false, Origin, BoxExtent);
	Vertices.Add(Origin);
	for (const FVector& Vert : Vertices)
	{
		GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, Vert, TraceChannel, TraceParams);
		if (!HitResult.bBlockingHit) return true;
	}

	return false;
}

bool UVisionConeComponent::GetTraceToLocation(const FVector& InLocation) const
{
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(GetOwner());
		
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, GetComponentLocation(), InLocation, TraceChannel, TraceParams);
	return !HitResult.bBlockingHit;
}

float UVisionConeComponent::GetAngleToActor(const AActor* InActor) const
{
	if (!InActor) return -1.0f;
	const FVector DotA = InActor->GetActorLocation() - GetComponentLocation(), DotB = GetForwardVector();
	return 180.0f / UE_DOUBLE_PI * FMath::Acos(FVector::DotProduct(DotA.GetSafeNormal(), DotB.GetSafeNormal()));
}

float UVisionConeComponent::GetAngleToLocation(const FVector& InLocation) const
{
	const FVector DotA = InLocation - GetComponentLocation(), DotB = GetForwardVector();
	return 180.0f / UE_DOUBLE_PI * FMath::Acos(FVector::DotProduct(DotA.GetSafeNormal(), DotB.GetSafeNormal()));
}

bool UVisionConeComponent::IsActorInRange(const AActor* InActor) const
{
	if (!InActor) return false;
	return FVector::Distance(GetComponentLocation(), InActor->GetActorLocation()) <= FMath::Max(MaxDistance, 10.0f);
}

bool UVisionConeComponent::IsLocationInRange(const FVector& InLocation) const
{
	return FVector::Distance(GetComponentLocation(), InLocation) <= FMath::Max(MaxDistance, 10.0f);
}

EVisionConeState UVisionConeComponent::GetActorVisionState(const AActor* InActor, const bool bCollidingOnly) const
{
	if (!IsActorInRange(InActor)) return EVisionConeState::None;

	const float AngleToTarget = GetAngleToActor(InActor);
	if (AngleToTarget >= 0.0f && AngleToTarget <= GetPeripheralAngle() && GetTraceToActor(InActor, bCollidingOnly))
	{
		return AngleToTarget <= GetBaseAngle() ? EVisionConeState::FullySeen : EVisionConeState::Peripheral;
	}
	
	return EVisionConeState::None;
}

EVisionConeState UVisionConeComponent::GetLocationVisionState(const FVector& InLocation) const
{
	if (!IsLocationInRange(InLocation)) return EVisionConeState::None;

	const float AngleToTarget = GetAngleToLocation(InLocation);
	if (AngleToTarget >= 0.0f && AngleToTarget <= GetPeripheralAngle() && GetTraceToLocation(InLocation))
	{
		return AngleToTarget <= GetBaseAngle() ? EVisionConeState::FullySeen : EVisionConeState::Peripheral;
	}
	
	return EVisionConeState::None;
}
