// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Actors/ToroCharacter.h"
#include "Player/PlayerCharacter.h"
#include "SMStateMachineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameEnemyBase.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	// No activity (no intensity)
	None = 0,

	// Enemy roaming (lowest intensity)
	Roaming = 1,

	// Enemy discreetly stalking player (low intensity)
	Stalking = 2,

	// Enemy investigating stimuli (medium intensity)
	Alerted = 3,

	// Enemy searching player after losing (high intensity)
	Hunting = 4,

	// Enemy chasing player (highest intensity)
	Chasing = 5
};
ENUM_RANGE_BY_FIRST_AND_LAST(EEnemyState, EEnemyState::None, EEnemyState::Chasing)

UCLASS(Abstract)
class FAFREVGAME_API AGameEnemyBase : public AToroCharacter
{
	GENERATED_BODY()

public:

	AGameEnemyBase();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USMStateMachineComponent> StateComponent;

	UFUNCTION(BlueprintCallable, Category = Enemy)
		virtual void SetEnemyState(const EEnemyState InState);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Enemy)
		void EnableAI(const EEnemyState StartState);
	void EnableAI_Implementation(const EEnemyState StartState);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Enemy)
		void DisableAI();
	void DisableAI_Implementation();

	UFUNCTION(BlueprintPure, Category = Enemy)
		USMInstance* GetStateMachine() const;

	UFUNCTION(BlueprintPure, Category = Enemy)
		bool IsEnemyState(const EEnemyState InState) const { return EnemyState == InState; }

	UFUNCTION(BlueprintPure, Category = Enemy)
		EEnemyState GetEnemyState() const { return EnemyState; }

	UFUNCTION(BlueprintPure, Category = Enemy)
		bool IsEnabled() const { return bEnabled; }

	UFUNCTION(BlueprintPure, Category = Enemy)
		APlayerCharacter* GetPlayerRef() const { return PlayerChar; }

protected:

	UPROPERTY(VisibleAnywhere, Category = Settings, AdvancedDisplay)
		EEnemyState EnemyState;

	bool bEnabled;
	TObjectPtr<APlayerCharacter> PlayerChar;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;
};
