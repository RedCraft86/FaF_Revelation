// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "Framework/ToroPlayerCharacter.h"
#include "GamePlayerCharacter.generated.h"

class UInteractionManager;
struct FInputActionValue;

UCLASS(Abstract, Blueprintable, BlueprintType)
class AGamePlayerCharacter final : public AToroPlayerCharacter
{
	GENERATED_BODY()

public:

	AGamePlayerCharacter();

	UInteractionManager* GetInteractionManager() const { return Interaction; }

private:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true))
		TObjectPtr<UInteractionManager> Interaction;

	virtual void BeginPlay() override;
	virtual void SlowTick() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void InputBinding_Pause(const FInputActionValue& InValue);
	void InputBinding_Back(const FInputActionValue& InValue);
	void InputBinding_Turn(const FInputActionValue& InValue);
	void InputBinding_Move(const FInputActionValue& InValue);
	void InputBinding_Run(const FInputActionValue& InValue);
	void InputBinding_Crouch(const FInputActionValue& InValue);
	void InputBinding_Lean(const FInputActionValue& InValue);
	void InputBinding_ToggleQuests(const FInputActionValue& InValue);
	void InputBinding_Interact(const FInputActionValue& InValue);
	void InputBinding_Equipment(const FInputActionValue& InValue);
};
