// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "LightProbe.h"
#include "ClassGetterHelpers.h"
#include "Subsystems/WorldSubsystem.h"
#include "UserSettings/MasterPostProcess.h"
#include "LightProbeManager.generated.h"

inline FName ParamName(const uint8 Idx, const bool bVec)
{
	return *FString::Printf(TEXT("%s_%d"),
		bVec ? TEXT("Scalar") : TEXT("Vector"), Idx + 1);
}

UCLASS()
class TORORUNTIME_API ULightProbeManager final : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:

	ULightProbeManager() : bDisabled(false), TickTime(0.0f) {}

	WORLD_SUBSYSTEM_GETTER(ULightProbeManager);
	
	void ForceProbeRecollection() { TickTime = 1.0f; }
	
private:

	UPROPERTY() bool bDisabled;
	UPROPERTY() float TickTime;
	UPROPERTY(Transient) TObjectPtr<AMasterPostProcess> MasterPP;
	UPROPERTY(Transient) TObjectPtr<APlayerCameraManager> CamManager;
	UPROPERTY(Transient) TArray<TObjectPtr<ALightProbe>> LightProbes;
	UPROPERTY(Transient) TObjectPtr<UMaterialParameterCollectionInstance> ProbeMPC;

	void UpdateProbes();
	void CollectProbes();
	void ResetMPC(const uint8 Idx) const;
	FTransform GetCamera() const;

	virtual bool IsTickable() const override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
#if WITH_EDITOR
	virtual bool IsTickableInEditor() const override { return true; }
#endif
};
