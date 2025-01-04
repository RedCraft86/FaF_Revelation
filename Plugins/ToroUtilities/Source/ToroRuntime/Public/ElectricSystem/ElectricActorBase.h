// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroActor.h"
#include "ElectricActorBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FElectricStateChangedSignature, const bool, bNewState);

UCLASS(Abstract)
class TORORUNTIME_API AElectricActorBase : public AToroActor
{
	GENERATED_BODY()

public:

	AElectricActorBase();

	UPROPERTY(EditAnywhere, Category = "Settings")
		uint8 MinEnergy;

	UPROPERTY(BlueprintAssignable, DisplayName = "On State Changed")
		FElectricStateChangedSignature OnStateChangedBP;

	UFUNCTION(BlueprintCallable, Category = "ElectricActor")
		void AddEnergy(const FName Key, const uint8 Value);

	UFUNCTION(BlueprintCallable, Category = "ElectricActor")
		void RemoveEnergy(const FName Key);

	UFUNCTION(BlueprintPure, Category = "ElectricActor")
		uint8 GetEnergy();

	UFUNCTION(BlueprintPure, Category = "ElectricActor")
		bool GetState();

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Energy Changed")
		void EnergyChangedEvent(const uint8 Total);
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "State Changed")
		void StateChangedEvent(const bool bState);
	
protected:

	UPROPERTY() bool bCachedState;
	UPROPERTY() TMap<FName, uint8> Energy;
	UPROPERTY() TOptional<uint8> CachedEnergy;
	
	virtual void OnEnergyChanged(const uint8 Total);
	virtual void OnStateChanged(const bool bInState);
	
	virtual void BeginPlay() override;
	virtual void OnEnableStateChanged(const bool bIsEnabled) override;
};
