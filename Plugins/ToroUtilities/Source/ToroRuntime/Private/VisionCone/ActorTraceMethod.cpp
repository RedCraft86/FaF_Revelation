// Copyright (C) RedCraft86. All Rights Reserved.

#include "VisionCone/ActorTraceMethod.h"

bool UActorTrace_Origin::TraceActor(const USceneComponent* Start, const AActor* Target)
{
	if (!Start || !Target) return false;
		
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Target);
	Params.AddIgnoredActor(Start->GetOwner());
	return !Start->GetWorld()->LineTraceSingleByChannel(Hit,
		Start->GetComponentLocation(), Target->GetActorLocation(), TraceChannel, Params);
}

bool UActorTrace_BoundingBox::TraceActor(const USceneComponent* Start, const AActor* Target)
{
	if (!Start || !Target) return false;

	FVector Origin, BoxExtent;
	const TArray<FVector> Vertices = UToroGeneralUtils::GetBoundingBoxVertices(Target,
		false, true, Origin, BoxExtent);
	const TArray<FVector> Vectors = CheckParams.ProcessVertices(Vertices, Origin);
		
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
