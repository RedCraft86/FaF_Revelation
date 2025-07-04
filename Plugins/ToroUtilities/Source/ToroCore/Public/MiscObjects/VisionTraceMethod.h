// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UObject/Object.h"
#include "Libraries/ToroGeneralUtils.h"
#include "VisionTraceMethod.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced)
class TOROCORE_API UVisionTraceMethod : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (DisplayPriority = -1))
		TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;

public:

	UFUNCTION(BlueprintImplementableEvent)
		bool TraceActor(const USceneComponent* Start, const AActor* Target);
	virtual bool TraceActor_Implementation(const USceneComponent* Start, const AActor* Target) { return false; }
};

UCLASS(NotBlueprintable, DisplayName = "Origin")
class TOROCORE_API UVisionTrace_Origin final : public UVisionTraceMethod
{
	GENERATED_BODY()

public:

	virtual bool TraceActor_Implementation(const USceneComponent* Start, const AActor* Target) override
	{
		if (Start && Target)
		{
			FHitResult Hit;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(Target);
			Params.AddIgnoredActor(Start->GetOwner());
			return !Start->GetWorld()->LineTraceSingleByChannel(Hit,
				Start->GetComponentLocation(), Target->GetActorLocation(), TraceChannel, Params);
		}

		return false;
	}
};

UCLASS(NotBlueprintable, DisplayName = "Bounding Box")
class TOROCORE_API UVisionTrace_BoundingBox final : public UVisionTraceMethod
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 0.0f, UIMin = 0.0f, ClampMax = 1.0f, UIMax = 1.0f))
		FVector BoundingBoxLerp;

	UPROPERTY(EditAnywhere, Category = Settings)
		bool bOnlyCollidingComponents;

	UPROPERTY(EditAnywhere, Category = Settings)
		bool bIncludeFromChildActors;

	TArray<FVector> ProcessVertices(const TArray<FVector>& InVertices, const FVector& Origin) const
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
	
public:

	virtual bool TraceActor_Implementation(const USceneComponent* Start, const AActor* Target) override
	{
		if (Start && Target)
		{
			FVector Origin, BoxExtent;
			const TArray<FVector> Vectors = ProcessVertices(UPrimitiveDataLibrary::GetBoundingBoxVertices(Target,
				bOnlyCollidingComponents, bIncludeFromChildActors, Origin, BoxExtent), Origin);
		
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
		}

		return false;
	}
};