// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (ClampMin = 100.0f, UIMin = 100.0f, ClampMax = 300.0f, UIMax = 300.0f))
		float ReachDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		TEnumAsByte<ECollisionChannel> InteractTrace;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		FVector2D Sensitivity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		FToroFloatMulti SensitivityMulti;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		FToroFloatSum FieldOfView;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (ClampMin = 0.1f, UIMin = 0.1f, ClampMax = 10.0f, UIMax = 10.0f))
		float FieldOfViewSpeed;

protected:
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void OnConstruction(const FTransform& Transform) override;
};
