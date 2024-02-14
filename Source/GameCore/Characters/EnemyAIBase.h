﻿// Copyright (C) RedCraft86 (Tayzar). All Rights Reserved.

#pragma once

#include "GameFramework/Character.h"
#include "SMStateMachineComponent.h"
#include "Interfaces/GCCharacterInterface.h"
#include "EnemyAIBase.generated.h"

UCLASS(DisplayName = "Enemy AI Base")
class GAMECORE_API AEnemyAIBase : public ACharacter, public IGCCharacterInterface
{
	GENERATED_BODY()

public:

	AEnemyAIBase();

	UPROPERTY(VisibleAnywhere, Category = "Logic")
		USMStateMachineComponent* LogicComponent;

	UFUNCTION(BlueprintImplementableEvent)
		const UArrowComponent* GetEyeArrowComponent() const;
	
	virtual FVector GetEyeWorldLocation_Implementation() override;
	virtual FVector GetEyeForwardVector_Implementation() override;
	virtual USceneComponent* GetLookAtComponent_Implementation() override { return nullptr; } 

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
