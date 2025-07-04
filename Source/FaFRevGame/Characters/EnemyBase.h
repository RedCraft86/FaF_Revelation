﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Actors/ToroCharacter.h"
#include "SMStateMachineComponent.h"
#include "EnemyBase.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	None,
	Roam,
	Suspect,
	Investigate,
	Search,
	Chase,
};

UCLASS(Abstract)
class FAFREVGAME_API AEnemyBase : public AToroCharacter
{
	GENERATED_BODY()

public:

	AEnemyBase();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USMStateMachineComponent> AIComponent;

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Jumpscare Player")
		void OnJumpscarePlayer() const;

	UFUNCTION(BlueprintCallable, Category = Game)
		void StopCharacterAI();

	UFUNCTION(BlueprintCallable, Category = Game)
		void StartCharacterAI();

	UFUNCTION(BlueprintCallable, Category = Character)
		void JumpscarePlayer();

	UFUNCTION(BlueprintPure, Category = Character)
		bool IsJumpscaring() const { return bJumpscare; }

	UFUNCTION(BlueprintCallable, Category = Game)
		virtual void SetEnemyState(const EEnemyState InState);

	UFUNCTION(BlueprintPure, Category = Game)
		EEnemyState GetEnemyState() const { return EnemyState; }

protected:

	UPROPERTY() bool bJumpscare;
	UPROPERTY() EEnemyState EnemyState;
};
