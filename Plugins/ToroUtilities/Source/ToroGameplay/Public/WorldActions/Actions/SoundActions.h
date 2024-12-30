﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Sound/AmbientSound.h"
#include "Components/AudioComponent.h"
#include "WorldActions/WorldActionBase.h"
#include "SoundActions.generated.h"

USTRUCT(BlueprintInternalUseOnly, DisplayName = "Sound Actions", meta = (Hidden))
struct TOROGAMEPLAY_API FWTaskSoundActions : public FWorldActionBase
{ GENERATED_BODY() };

USTRUCT(BlueprintType, DisplayName = "World Sound")
struct TOROGAMEPLAY_API FWTaskWorldSound final : public FWTaskSoundActions
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = WorldSound)
		bool bStopSound;
	
	UPROPERTY(EditAnywhere, Category = WorldSound, meta = (ClampMin = 0.0f, UIMin = 0.0f, EditCondition = "!bStopSound", EditConditionHides))
		float StartTime;
	
	UPROPERTY(EditAnywhere, Category = "WorldSound|Fade")
		bool bFade;

	UPROPERTY(EditAnywhere, Category = "WorldSound|Fade", meta = (ClampMin = 0.1f, UIMin = 0.1f, EditCondition = "bFade", EditConditionHides))
		float FadeTime;
	
	UPROPERTY(EditAnywhere, Category = "WorldSound|Fade", meta = (EditCondition = "bFade", EditConditionHides))
		EAudioFaderCurve FadeCurve;

	UPROPERTY(EditAnywhere, Category = WorldSound)
		TSet<TSoftObjectPtr<AAmbientSound>> Targets;

	FWTaskWorldSound()
		: bStopSound(false), StartTime(0.0f), bFade(false), FadeTime(1.0f)
		, FadeCurve(EAudioFaderCurve::Linear), Targets({})
	{}

protected:
	
	virtual void RunEvent(const UObject* WorldContext) override;
};

USTRUCT(BlueprintType, DisplayName = "2D Sound")
struct TOROGAMEPLAY_API FWTaskSound2D final : public FWTaskSoundActions
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
