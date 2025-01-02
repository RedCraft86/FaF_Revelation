// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "FMODAmbientSound.h"
#include "WorldActions/WorldActionBase.h"
#include "SoundActions.generated.h"

USTRUCT(BlueprintInternalUseOnly, DisplayName = "Sound Actions", meta = (Hidden))
struct TORORUNTIME_API FWTaskSoundActions : public FWorldActionBase
{ GENERATED_BODY() };

USTRUCT(BlueprintType, DisplayName = "World Sound")
struct TORORUNTIME_API FWTaskWorldSound final : public FWTaskSoundActions
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = WorldSound)
		bool bStopSound;

	UPROPERTY(EditAnywhere, Category = WorldSound)
	// ReSharper disable once UnrealHeaderToolError
	TSet<TSoftObjectPtr<AFMODAmbientSound>> Targets;

	FWTaskWorldSound() : bStopSound(false) {}

protected:
	
	virtual void RunEvent(const UObject* WorldContext) override;
};

USTRUCT(BlueprintType, DisplayName = "2D Sound")
struct TORORUNTIME_API FWTaskSound2D final : public FWTaskSoundActions
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Sound2D)
		TObjectPtr<USoundBase> Sound;

	UPROPERTY(EditAnywhere, Category = Sound2D, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float Volume;
	
	UPROPERTY(EditAnywhere, Category = Sound2D, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float Pitch;

	FWTaskSound2D() : Volume(1.0f), Pitch(1.0f) {}

protected:

	virtual void RunEvent(const UObject* WorldContext) override;
};
