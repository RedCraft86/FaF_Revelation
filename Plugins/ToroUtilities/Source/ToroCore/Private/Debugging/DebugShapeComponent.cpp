// Copyright (C) RedCraft86. All Rights Reserved.

#include "Debugging/DebugShapeComponent.h"

UDebugShapeComponent::UDebugShapeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bIsEditorOnly = true;
#if WITH_EDITORONLY_DATA
	SetIsVisualizationComponent(true);
#endif
}

FVector UDebugShapeComponent::TransformLocation(const FVector& LocalLocation) const
{
	return GetOwner()->GetActorTransform().TransformPosition(LocalLocation);
}

FRotator UDebugShapeComponent::TransformRotation(const FRotator& LocalRotation) const
{
	return GetOwner()->GetActorTransform().TransformRotation(LocalRotation.Quaternion()).Rotator();
}

FVector UDebugShapeComponent::AsForwardVector(const FRotator& LocalRotation) const
{
	return TransformRotation(LocalRotation).Vector();
}

FVector UDebugShapeComponent::AsRightVector(const FRotator& LocalRotation) const
{
	return FRotationMatrix(TransformRotation(LocalRotation)).GetScaledAxis(EAxis::Y);
}

FVector UDebugShapeComponent::AsUpVector(const FRotator& LocalRotation) const
{
	return FRotationMatrix(TransformRotation(LocalRotation)).GetScaledAxis(EAxis::Z);
}

void UDebugShapeComponent::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		DestroyComponent();
	});
}