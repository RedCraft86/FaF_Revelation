// Copyright (C) RedCraft86. All Rights Reserved.

#include "Components/DebugShapeComponent.h"
#if WITH_EDITOR
#include "NavigationSystem.h"
#include "NavigationPath.h"
#endif

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
	return GetOwner()->GetActorTransform().TransformPositionNoScale(LocalLocation);
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

#if WITH_EDITOR
void UDebugShapeComponent::UpdateNavPoints()
{
	NavPathPoints.Empty();
	
	const int32 NumPoints = DebugNavPath.Targets.Num();
	if (NumPoints < 2) return;

	for (int i = 0; i < NumPoints - 1; i++)
	{
		if (const UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this,
			TransformLocation(DebugNavPath.Targets[i]), TransformLocation(DebugNavPath.Targets[i + 1])))
		{
			NavPathPoints.Append(NavPath->PathPoints);
		}
	}
}
#endif
