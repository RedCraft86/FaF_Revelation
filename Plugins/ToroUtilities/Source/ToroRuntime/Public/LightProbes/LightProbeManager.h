// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "LightProbe.h"
#include "ClassGetterHelpers.h"
#include "Subsystems/WorldSubsystem.h"
#include "LightProbeManager.generated.h"

inline FName ParamName(const uint8 Idx, const bool bPos)
{
	return *FString::Printf(TEXT("%s_%d"),
		bPos ? TEXT("Position") : TEXT("Color"), Idx + 1);
}

UCLASS()
class TORORUNTIME_API ULightProbeManager final : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:

	ULightProbeManager() : bHasLumen(false), TickTime(0.0f), CachedMax(0) {}
	WORLD_SUBSYSTEM_GETTER(ULightProbeManager);
	
	void ForceProbeRecollection();
	
private:

	UPROPERTY() bool bHasLumen;
	UPROPERTY() float TickTime;
	UPROPERTY() uint8 CachedMax;
	UPROPERTY(Transient) TArray<TWeakObjectPtr<ALightProbe>> LightProbes;
	UPROPERTY(Transient) TObjectPtr<UMaterialInstanceDynamic> ProbePPM;
	UPROPERTY(Transient) TObjectPtr<class AMasterPostProcess> MasterPP;
	UPROPERTY(Transient) TObjectPtr<APlayerCameraManager> CamManager;

	void UpdateProbes();
	void CollectProbes();
	void ResetPPM(const uint8 Idx) const;
	uint8 GetIterationMax() const;
	FTransform GetCamera() const;

	virtual bool IsTickable() const override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual TStatId GetStatId() const override { return GetStatID(); }
#if WITH_EDITOR
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual bool IsTickableInEditor() const override { return true; }
#endif
};
