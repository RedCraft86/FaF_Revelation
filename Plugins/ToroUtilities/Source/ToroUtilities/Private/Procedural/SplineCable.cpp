// Copyright (C) RedCraft86. All Rights Reserved.

#include "Procedural/SplineCable.h"

void ASplineCable::Construct()
{
	for (const TObjectPtr<USplineMeshComponent>& Comp : SplineMeshComponents) if (Comp) Comp->DestroyComponent();
	for (const TObjectPtr<UInstancedStaticMeshComponent>& Comp : AttachMeshComponents) if (Comp) Comp->DestroyComponent();
	if (StartMeshComponent) StartMeshComponent->DestroyComponent();
	if (EndMeshComponent) EndMeshComponent->DestroyComponent();
	
	AttachMeshComponents.Empty(AttachmentSample.Num());
	SplineMeshComponents.Empty();

	const int SplinePointCount = SplineComponent->GetNumberOfSplinePoints() - (bClosedLoop ? 0 : 1);
	
	if (CableMesh.IsValidData())
	{
		for (int32 i = 0; i < SplinePointCount; i++)
		{
			if (USplineMeshComponent* NewSplineMesh = AddGenericComponent<USplineMeshComponent>())
			{
				CableMesh.ToStaticMeshComponent(NewSplineMesh);
				Collision.ToPrimitiveComponent(NewSplineMesh);

				FVector Location, Tangent, NextLocation, NextTangent;
				SplineComponent->GetLocalLocationAndTangentAtSplinePoint(i, Location, Tangent);
				if (bClosedLoop && i == SplinePointCount - 1)
				{
					SplineComponent->GetLocalLocationAndTangentAtSplinePoint(0, NextLocation, NextTangent);
				}
				else
				{
					SplineComponent->GetLocalLocationAndTangentAtSplinePoint(i + 1, NextLocation, NextTangent);
				}
				
				NewSplineMesh->SetForwardAxis(CableMesh.MeshAxis, true);
				NewSplineMesh->SetStartAndEnd(Location, Tangent,
					NextLocation, NextTangent, true);
				
				SplineMeshComponents.Add(NewSplineMesh);
			}
		}
	}

	if (StartCap.IsValidData())
	{
		if (StartMeshComponent = AddGenericComponent<UStaticMeshComponent>(); StartMeshComponent)
		{
			StartCap.ToStaticMeshComponent(StartMeshComponent);
			Collision.ToPrimitiveComponent(StartMeshComponent);

			FVector Location, Tangent;
			SplineComponent->GetLocalLocationAndTangentAtSplinePoint(0, Location, Tangent);
			
			StartMeshComponent->SetRelativeTransform({
				StartCap.Transform.GetRotation().Rotator() + FRotationMatrix::MakeFromX(Tangent).Rotator(),
				StartCap.Transform.GetTranslation() + Location,
				StartCap.Transform.GetScale3D()
			});
		}
	}

	if (EndCap.IsValidData())
	{
		if (EndMeshComponent = AddGenericComponent<UStaticMeshComponent>(); EndMeshComponent)
		{
			EndCap.ToStaticMeshComponent(EndMeshComponent);
			Collision.ToPrimitiveComponent(EndMeshComponent);

			FVector Location, Tangent;
			SplineComponent->GetLocalLocationAndTangentAtSplinePoint(
				SplineComponent->GetNumberOfSplinePoints() - 1, Location, Tangent);
			
			EndMeshComponent->SetRelativeTransform({
				EndCap.Transform.GetRotation().Rotator() + FRotationMatrix::MakeFromX(Tangent).Rotator(),
				EndCap.Transform.GetTranslation() + Location,
				EndCap.Transform.GetScale3D()
			});
		}
	}

	for (int i = 0; i < AttachmentSample.Num(); i++)
	{
		UInstancedStaticMeshComponent* NewComp = AddGenericComponent<UInstancedStaticMeshComponent>();
		AttachmentSample[i].ToStaticMeshComponent(NewComp);
		Collision.ToPrimitiveComponent(NewComp);
		AttachMeshComponents.Add(NewComp);
	}

	if (AttachmentIndexes.Num() != AttachmentCount)
	{
		AttachmentIndexes = LoopMeshArray(AttachmentSample, AttachmentLoopMode, AttachmentCount);
	}
	
	const float Interval = SplineComponent->GetSplineLength() / (AttachmentCount + 1);
	for (int32 i = 0; i < AttachmentIndexes.Num(); i++)
	{
		const int32 idx = AttachmentIndexes[i];
		if (AttachmentSample.IsValidIndex(idx) && AttachMeshComponents.IsValidIndex(idx))
		{
			const FVector Location = SplineComponent->GetLocationAtDistanceAlongSpline(
				(i + 1) * Interval, ESplineCoordinateSpace::Local);
			
			const FRotator Rotation = SplineComponent->GetRotationAtDistanceAlongSpline(
				(i + 1) * Interval, ESplineCoordinateSpace::Local);

			const FTransform BaseTransform = AttachmentSample[idx].Transform;
			AttachMeshComponents[idx]->AddInstance({
				BaseTransform.GetRotation().Rotator() + Rotation,
				BaseTransform.GetTranslation() + Location,
				BaseTransform.GetScale3D()
			});
		}
	}
	
	Super::Construct();
}

#if WITH_EDITOR
void ASplineCable::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, AttachmentSample)
		|| PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, AttachmentLoopMode)
		|| PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, AttachmentCount))
	{
		AttachmentIndexes = LoopMeshArray(AttachmentSample, AttachmentLoopMode, AttachmentCount);
		Construct();
	}
}
#endif
