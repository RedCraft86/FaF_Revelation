// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UObject/Object.h"
#include "ToroGeneralUtils.h"
#include "ActorTraceMethod.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced)
class TORORUNTIME_API UActorTraceMethod : public UObject
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		TEnumAsByte<ECollisionChannel> TraceChannel;

public:

	virtual bool TraceActor(const USceneComponent* Start, const AActor* Target)
	{
		return false;
	}
};

UCLASS(NotBlueprintable, DisplayName = "Origin")
class TORORUNTIME_API UActorTrace_Origin final : public UActorTraceMethod
{
	GENERATED_BODY()

public:

	virtual bool TraceActor(const USceneComponent* Start, const AActor* Target) override;
};

UCLASS(NotBlueprintable, DisplayName = "Bounding Box")
class TORORUNTIME_API UActorTrace_BoundingBox final : public UActorTraceMethod
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Settings)
		FActorBoundsCheckParams CheckParams;

public:

	virtual bool TraceActor(const USceneComponent* Start, const AActor* Target) override;
};
