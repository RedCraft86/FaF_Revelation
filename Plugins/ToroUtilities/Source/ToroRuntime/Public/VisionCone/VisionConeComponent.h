// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ActorTraceMethod.h"
#include "Components/ArrowComponent.h"
#include "VisionConeComponent.generated.h"

UENUM(BlueprintType)
enum class EVisionState : uint8
{
	None,
	Peripheral,
	Sighted
};

UCLASS(NotBlueprintable, ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class TORORUNTIME_API UVisionConeComponent final : public UArrowComponent
{
	GENERATED_BODY()

public:

	UVisionConeComponent();

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 10.0f, UIMin = 10.0f))
		float Distance;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 10.0f, UIMin = 10.0f, ClampMax = 90.0f, UIMax = 90.0f))
		float ViewAngle;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 0.0f, UIMin = 0.0f))
		float PeripheralAngle;

	UPROPERTY(EditAnywhere, Category = Settings, Instanced)
		TObjectPtr<UActorTraceMethod> TraceMethod;

	UFUNCTION(BlueprintPure, Category = VisionCone)
		bool GetTraceToActor(const AActor* InActor) const;
	
	UFUNCTION(BlueprintPure, Category = VisionCone)
		float GetAngleToActor(const AActor* InActor) const;
	
	UFUNCTION(BlueprintPure, Category = VisionCone)
		bool IsActorInRange(const AActor* InActor) const;

	UFUNCTION(BlueprintPure, Category = VisionCone)
		EVisionState GetActorVisionState(const AActor* InActor) const;
	
	float GetViewAngle() const
	{
		return FMath::Max(0.0f, FMath::Min(ViewAngle, 90.0f));
	}
	
	float GetPeripheralAngle() const
	{
		return FMath::Max(0.0f, FMath::Min(GetViewAngle() + PeripheralAngle, 90.0f));
	}
};
