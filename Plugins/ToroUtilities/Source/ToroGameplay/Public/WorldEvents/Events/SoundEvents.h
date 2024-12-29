// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Sound/AmbientSound.h"
#include "Components/AudioComponent.h"
#include "WorldEvents/WorldEventBase.h"
#include "SoundEvents.generated.h"

USTRUCT(BlueprintInternalUseOnly, DisplayName = "Sound")
struct TOROGAMEPLAY_API FWESoundBase : public FWorldEventBase
{ GENERATED_BODY() };

USTRUCT(BlueprintType, DisplayName = "Ambient Sound")
struct TOROGAMEPLAY_API FWEAmbientSound final : public FWESoundBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "AmbientSound")
		bool bStopSound;
	
	UPROPERTY(EditAnywhere, Category = "AmbientSound", meta = (ClampMin = 0.0f, UIMin = 0.0f, EditCondition = "!bStopSound", EditConditionHides))
		float StartTime;
	
	UPROPERTY(EditAnywhere, Category = "AmbientSound")
		bool bFade;

	UPROPERTY(EditAnywhere, Category = "AmbientSound", meta = (ClampMin = 0.1f, UIMin = 0.1f, EditCondition = "bFade", EditConditionHides))
		float FadeTime;
	
	UPROPERTY(EditAnywhere, Category = "AmbientSound", meta = (EditCondition = "bFade", EditConditionHides))
		EAudioFaderCurve FadeCurve;

	UPROPERTY(EditAnywhere, Category = "AmbientSound")
		TSet<TSoftObjectPtr<AAmbientSound>> Targets;
	
	FWEAmbientSound()
		: bStopSound(false), StartTime(0.0f), bFade(false), FadeTime(1.0f)
		, FadeCurve(EAudioFaderCurve::Linear), Targets({})
	{}

protected:
	
	virtual void RunEvent(const UObject* WorldContext) override;
};

USTRUCT(BlueprintType, DisplayName = "2D Sound")
struct TOROGAMEPLAY_API FWESound2D final : public FWESoundBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "PlaySound2D")
		TObjectPtr<USoundBase> Sound;

	UPROPERTY(EditAnywhere, Category = "PlaySound2D", meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float Volume;
	
	UPROPERTY(EditAnywhere, Category = "PlaySound2D", meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float Pitch;

	FWESound2D() : Volume(1.0f), Pitch(1.0f) {}

protected:
	
	virtual void RunEvent(const UObject* WorldContext) override;
};