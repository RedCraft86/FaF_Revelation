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

	ULightProbeManager() : bNewMat(true), bDisabled(false), TickTime(0.0f) {}

	WORLD_SUBSYSTEM_GETTER(ULightProbeManager);
	
	void ForceProbeRecollection();
	UMaterialInstanceDynamic* GetPostProcessMat() const { return ProbePPM; }
	
private:

	UPROPERTY() bool bNewMat;
	UPROPERTY() bool bDisabled;
	UPROPERTY() float TickTime;
	UPROPERTY(Transient) TArray<TObjectPtr<ALightProbe>> LightProbes;
	UPROPERTY(Transient) TObjectPtr<APlayerCameraManager> CamManager;
	UPROPERTY(Transient) TObjectPtr<UMaterialInstanceDynamic> ProbePPM;
	UPROPERTY(Transient) TObjectPtr<class AMasterPostProcess> MasterPP;

	void UpdateProbes();
	void CollectProbes();
	void ResetPPM(const uint8 Idx) const;
	uint8 GetIterationMax() const;
	FTransform GetCamera() const;

	virtual bool IsTickable() const override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
#if WITH_EDITOR
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual bool IsTickableInEditor() const override { return true; }
#endif
};
