// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "LightProbeBase.generated.h"

/* Base class for the Light Probe system to fake GI.
 * This handles automatic disabling of probes if Lumen is enabled.
 */
UCLASS()
class TORORUNTIME_API ALightProbeBase final : public AActor
{
	GENERATED_BODY()

	friend class AMasterPostProcess;;
	
public:
	
	ALightProbeBase();

	UPROPERTY(EditAnywhere, Category = Default)
		bool bDisableProbe;
	
	/* If enabled, the probe will turn itself off if Lumen GI is enabled. */
	UPROPERTY(EditAnywhere, Category = Default)
		bool bAffectedByLumenGI;

	UFUNCTION(BlueprintPure, Category = LightProbe)
		bool ShouldDisable() const;

protected:

	UPROPERTY() bool bUsingLumen;
	virtual void BeginPlay() override;
	void OnSettingChanged(const class UToroUserSettings* InSettings);
};
