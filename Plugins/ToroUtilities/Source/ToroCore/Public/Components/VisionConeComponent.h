// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Components/SceneComponent.h"
#include "MiscObjects/VisionTraceMethod.h"
#include "VisionConeComponent.generated.h"

UENUM(BlueprintType)
enum class EVisionState : uint8
{
	None,
	Peripheral,
	Sighted
};

UCLASS(NotBlueprintable, ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class TOROCORE_API UVisionConeComponent final : public USceneComponent
{
	GENERATED_BODY()

public:

	UVisionConeComponent();

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 10.0f, UIMin = 10.0f))
		float Distance;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 10.0f, UIMin = 10.0f, ClampMax = 90.0f, UIMax = 90.0f))
		float OuterAngle;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 0.0f, UIMin = 0.0f))
		float InnerAngle;

	UPROPERTY(EditAnywhere, Category = Settings, Instanced)
		TObjectPtr<UVisionTraceMethod> TraceMethod;

	UFUNCTION(BlueprintPure, Category = VisionCone)
		bool GetTraceToActor(const AActor* InActor) const;

	UFUNCTION(BlueprintPure, Category = VisionCone)
		float GetAngleToActor(const AActor* InActor) const;

	UFUNCTION(BlueprintPure, Category = VisionCone)
		bool IsActorInRange(const AActor* InActor) const;

	UFUNCTION(BlueprintPure, Category = VisionCone)
		EVisionState GetActorVisionState(const AActor* InActor) const;

	float GetOuterAngle() const
	{
		return FMath::Max(0.0f, FMath::Min(OuterAngle, 90.0f));
	}

	float GetInnerAngle() const
	{
		return FMath::Max(0.0f, FMath::Min(GetOuterAngle() + InnerAngle, 90.0f));
	}
};
