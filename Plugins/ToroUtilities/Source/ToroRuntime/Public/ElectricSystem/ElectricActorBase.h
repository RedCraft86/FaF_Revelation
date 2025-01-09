// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#if WITH_EDITOR
#include "DebugIconComponent.h"
#endif
#include "GameFramework/Actor.h"
#include "ElectricActorBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FElectricStateChangedSignature, const bool, bNewState);

UCLASS(Abstract, meta = (HiddenCategories = "Collision"))
class TORORUNTIME_API AElectricActorBase : public AActor
{
	GENERATED_BODY()

public:

	AElectricActorBase();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

#if WITH_EDITORONLY_DATA
	UPROPERTY() TObjectPtr<UDebugIconComponent> DebugIcon;
#endif

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = Settings)
		bool bPreviewState = true;
#endif

	UPROPERTY(EditAnywhere, Category = Settings)
		uint8 MinEnergy;

	UPROPERTY(EditAnywhere, Category = Settings, AdvancedDisplay)
		bool bRequiresCollision;
	
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
#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};
