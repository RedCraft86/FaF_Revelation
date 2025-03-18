// Copyright (C) RedCraft86. All Rights Reserved.

#include "VisionTraceMethod.h"
#include "ToroGeneralUtils.h"

bool UVisionTrace_Origin::TraceActor_Implementation(const USceneComponent* Start, const AActor* Target)
{
	if (!Start || !Target) return false;
		
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Target);
	Params.AddIgnoredActor(Start->GetOwner());
	return !Start->GetWorld()->LineTraceSingleByChannel(Hit,
		Start->GetComponentLocation(), Target->GetActorLocation(), TraceChannel, Params);
}

TArray<FVector> UVisionTrace_BoundingBox::ProcessVertices(const TArray<FVector>& InVertices, const FVector& Origin) const
{
	TArray<FVector> TestVectors {Origin};
	for (const FVector& Vector : InVertices)
	{
		TestVectors.AddUnique(FVector(
			FMath::Lerp(Origin.X, Vector.X, BoundingBoxLerp.X),
			FMath::Lerp(Origin.Y, Vector.Y, BoundingBoxLerp.Y),
			FMath::Lerp(Origin.Z, Vector.Z, BoundingBoxLerp.Z)
		));
	}

	return TestVectors;
}

bool UVisionTrace_BoundingBox::TraceActor_Implementation(const USceneComponent* Start, const AActor* Target)
{
	if (!Start || !Target) return false;

	FVector Origin, BoxExtent;
	const TArray<FVector> Vertices = UToroGeneralUtils::GetBoundingBoxVertices(Target,
		bOnlyCollidingComponents, bIncludeFromChildActors, Origin, BoxExtent);
	const TArray<FVector> Vectors = ProcessVertices(Vertices, Origin);
		
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Target);
	Params.AddIgnoredActor(Start->GetOwner());
	const FVector StartLoc = Start->GetComponentLocation();
	for (const FVector& Vector : Vectors)
	{
		if (!Start->GetWorld()->LineTraceSingleByChannel(Hit, StartLoc, Vector, TraceChannel, Params))
			return true;
	}

	return false;
}