// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InlineCurves.h"
#include "Characters/ToroPlayerBase.h"
#include "DataTypes/AdvancedFloatTypes.h"
#include "Interaction/InteractionComponent.h"
#include "ToroFirstPersonPlayer.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API AToroFirstPersonPlayer : public AToroPlayerBase
{
	GENERATED_BODY()

public:
	
	AToroFirstPersonPlayer();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class USpringArmComponent> CameraArm;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UCameraComponent> PlayerCamera;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> EquipmentRoot;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UInteractionComponent> Interaction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (ClampMin = 10.0f, UIMin = 100.0f, UIMax = 300.0f))
		float ReachDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
		TEnumAsByte<ECollisionChannel> InteractTrace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
		FToroFloatSum FieldOfView;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (ClampMin = 0.1f, UIMin = 0.1f, UIMax = 25.0f))
		float FieldOfViewSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Settings, meta = (DisplayThumbnail = false))
		TSoftObjectPtr<USceneComponent> LockOnTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (ClampMin = 5.0f, UIMin = 5.0f, UIMax = 25.0f))
		float LockOnSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
		FVector2D Sensitivity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
		FToroFloatMulti SensitivityMulti;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		FVector2D LeanOffsets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float LeanSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		TEnumAsByte<ECollisionChannel> SideTrace;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		float SideTraceLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
		FToroFloatMulti MoveSpeedMulti;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (ClampMin = 10.0f, UIMin = 10.0f, UIMax = 1000.0f))
		float WalkingSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
		FVector2D SwayOffsets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (ClampMin = 10.0f, UIMin = 10.0f, UIMax = 1000.0f))
		float CrouchWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (ClampMin = -10.0f, UIMin = -10.0f, ClampMax = 10.0f, UIMax = 10.0f))
		float CrouchFOV;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
		TEnumAsByte<ECollisionChannel> CeilingTrace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
		FVector2D CrouchHeights;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (ClampMin = 0.1f, UIMin = 0.1f, UIMax = 2.0f))
		float CrouchRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
		FInlineFloatCurve CrouchAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (ClampMin = 10.0f, UIMin = 10.0f, UIMax = 1000.0f))
		float RunningSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (ClampMin = -10.0f, UIMin = -10.0f, ClampMax = 10.0f, UIMax = 10.0f))
		float RunningFOV;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (ClampMin = 1.0f, UIMin = 1.0f, UIMax = 200.0f))
		float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
		FToroFloatMulti StaminaDrainRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
		FToroFloatMulti StaminaGainRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, AdvancedDisplay)
		FPlayerCameraShakes CameraShakes;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, AdvancedDisplay)
		FPlayerFootsteps Footsteps;

protected:
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void OnConstruction(const FTransform& Transform) override;
};
