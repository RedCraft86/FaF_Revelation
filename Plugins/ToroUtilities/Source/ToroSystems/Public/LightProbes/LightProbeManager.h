// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "LightProbeActor.h"
#include "Helpers/ClassGetters.h"
#include "Subsystems/WorldSubsystem.h"
#include "LightProbeManager.generated.h"

inline FName ProbeParam(const uint8 Idx, const bool bPos)
{
	return *FString::Printf(TEXT("%s_%d"),
		bPos ? TEXT("Position") : TEXT("Color"), Idx + 1);
}

UCLASS(BlueprintType, DisplayName = "Light Probe Manager")
class TOROSYSTEMS_API ULightProbeManager final : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:

	ULightProbeManager(): bHasLumen(false), TickTime(0.0f) {}

	WORLD_SUBSYSTEM_GETTER(ULightProbeManager);

	void ForceProbeRecollection() { TickTime = 1.0f; }
	
private:

	UPROPERTY() bool bHasLumen;
	UPROPERTY() float TickTime;
	UPROPERTY(Transient) TObjectPtr<APlayerCameraManager> CamManager;
	UPROPERTY(Transient) TObjectPtr<class AMasterPostProcess> MasterPP;
	UPROPERTY(Transient) TArray<TWeakObjectPtr<ALightProbeActor>> LightProbes;

	void UpdateProbes();
	void CollectProbes();
	void ResetPPM(const uint8 Idx) const;
	FTransform GetCamera() const;

	virtual bool IsTickable() const override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual TStatId GetStatId() const override { return GetStatID(); }
#if WITH_EDITOR
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual bool IsTickableInEditor() const override { return true; }
#endif
};
