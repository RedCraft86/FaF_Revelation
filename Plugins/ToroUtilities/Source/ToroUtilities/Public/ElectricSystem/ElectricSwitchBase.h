// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroActor.h"
#include "ElectricActorBase.h"
#include "ElectricSwitchBase.generated.h"

UCLASS(Abstract)
class TOROUTILITIES_API AElectricSwitchBase : public AToroActor
{
	GENERATED_BODY()

public:

	AElectricSwitchBase();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		bool bState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (ClampMin = 1, UIMin = 1))
		uint8 PowerValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		TSet<TObjectPtr<AElectricActorBase>> Targets;

	UPROPERTY(BlueprintAssignable, DisplayName = "On State Changed")
		FElectricStateChangedSignature OnStateChangedBP;
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "State Changed")
		void StateChangedEvent(const bool bNewState);

	UFUNCTION(BlueprintCallable, Category = ElectricSwitch)
		void SetState(const bool bNewState);

	UFUNCTION(BlueprintCallable, Category = ElectricSwitch)
		void ToggleState();

protected:

	void UpdateStates();
	
	virtual void BeginPlay() override;
	virtual void OnEnableStateChanged(const bool bIsEnabled) override;
};
