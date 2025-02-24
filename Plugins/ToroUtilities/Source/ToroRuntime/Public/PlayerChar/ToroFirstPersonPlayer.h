// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Characters/ToroPlayerBase.h"
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
		TObjectPtr<UAudioComponent> FootstepAudio;

protected:
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
};
