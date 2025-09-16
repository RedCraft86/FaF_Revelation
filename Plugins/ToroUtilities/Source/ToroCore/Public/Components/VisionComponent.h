// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Components/SceneComponent.h"
#include "VisionComponent.generated.h"

UENUM(BlueprintType)
enum class EVisionState : uint8
{
	None,
	Peripheral,
	Sighted
};

USTRUCT(BlueprintType)
struct TOROCORE_API FVisionSettings
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 10.0f, UIMin = 10.0f, ClampMax = 5000.0f, UIMax = 5000.0f))
		float Distance;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 10.0f, UIMin = 10.0f, ClampMax = 90.0f, UIMax = 90.0f))
		float OuterAngle;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 0.0f, UIMin = 0.0f, ClampMax = 89.0f, UIMax = 89.0f))
		float InnerAngle;

	FVisionSettings()
		: Distance(2000.0f)
		, OuterAngle(60.0f)
		, InnerAngle(20.0f)
	{}
};

UCLASS(NotBlueprintable, ClassGroup = (AI), meta = (BlueprintSpawnableComponent))
class TOROCORE_API UVisionComponent final : public USceneComponent
{
	GENERATED_BODY()

public:

	UVisionComponent();

	/* Settings for the Vision Cone shape. */
	UPROPERTY(EditAnywhere, Category = Settings)
		FVisionSettings Vision;

	/* Line trace channel that should be used when occlusion checking. */
	UPROPERTY(EditAnywhere, Category = Settings)
		TEnumAsByte<ECollisionChannel> TraceChannel;

	/* Whether Character type actors should automatically be detected. (Use On Character State delegates for updates) */
	UPROPERTY(EditAnywhere, Category = Settings)
		bool bDetectCharacters;

	/* Whether an actor is within this vision cone at which state. */
	UFUNCTION(BlueprintPure, Category = VisionCone)
		EVisionState GetActorVisionState(const AActor* InActor) const;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterStateBP, const ACharacter*, Char, EVisionState, State);
	UPROPERTY(BlueprintAssignable, DisplayName = "On Character State")
		FOnCharacterStateBP OnCharacterStateBP;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCharacterState, const ACharacter*, EVisionState);
	FOnCharacterState OnCharacterState;

private:

	TMap<TWeakObjectPtr<const ACharacter>, EVisionState> DetectionCache;

	bool GetTraceToActor(const AActor* InActor) const;
	float GetAngleToActor(const AActor* InActor) const;
	bool IsActorInRange(const AActor* InActor) const;

	float GetOuterAngle() const { return FMath::Clamp(Vision.OuterAngle, 0.0f, 90.0f); }
	float GetInnerAngle() const { return FMath::Clamp(Vision.InnerAngle, 0.0f, GetOuterAngle() - 1.0f); }

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
