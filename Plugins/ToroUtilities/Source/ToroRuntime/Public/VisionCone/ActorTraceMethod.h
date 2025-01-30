// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UObject/Object.h"
#include "ActorTraceMethod.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced)
class TORORUNTIME_API UActorTraceMethod : public UObject
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (DisplayPriority = -1))
		TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;

public:

	UFUNCTION(BlueprintImplementableEvent)
		bool TraceActor(const USceneComponent* Start, const AActor* Target);
	virtual bool TraceActor_Implementation(const USceneComponent* Start, const AActor* Target)
	{
		return false;
	}
};

UCLASS(NotBlueprintable, DisplayName = "Origin")
class TORORUNTIME_API UActorTrace_Origin final : public UActorTraceMethod
{
	GENERATED_BODY()

public:

	virtual bool TraceActor_Implementation(const USceneComponent* Start, const AActor* Target) override;
};

UCLASS(NotBlueprintable, DisplayName = "Bounding Box")
class TORORUNTIME_API UActorTrace_BoundingBox final : public UActorTraceMethod
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 0.0f, UIMin = 0.0f, ClampMax = 1.0f, UIMax = 1.0f))
		FVector BoundingBoxLerp;

	UPROPERTY(EditAnywhere, Category = Settings)
		bool bOnlyCollidingComponents;

	UPROPERTY(EditAnywhere, Category = Settings)
		bool bIncludeFromChildActors;

	TArray<FVector> ProcessVertices(const TArray<FVector>& InVertices, const FVector& Origin) const;
	
public:

	virtual bool TraceActor_Implementation(const USceneComponent* Start, const AActor* Target) override;
};
