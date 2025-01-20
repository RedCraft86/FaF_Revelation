// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroVolume.h"
#include "WorldMusicManager.h"
#include "WorldMusicVolume.generated.h"

UCLASS()
class TORORUNTIME_API AWorldMusicVolume final : public AToroVolume
{
	GENERATED_BODY()

public:
	
	AWorldMusicVolume();
	
	UPROPERTY(EditAnywhere, Category = Settings)
		bool bSingleUse;
	
	UPROPERTY(EditAnywhere, Category = Settings)
		bool bStopOnExit;
	
	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 1.0f))
		float Cooldown;

	UPROPERTY(EditAnywhere, Category = Settings, NoClear)
		TObjectPtr<USoundBase> Sound;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (EditCondition = "Sound != nullptr", ClampMin = 0.1f))
		float Volume;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (EditCondition = "Sound != nullptr", ClampMin = 0.1f))
		float FadeTime;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (EditCondition = "Sound != nullptr", ClampMin = 0.0f))
		FVector2D StartRange;

private:

	UPROPERTY() bool bHasPlayed;
	UPROPERTY() FTimerHandle CooldownTimer;
	UPROPERTY(Transient) TObjectPtr<APawn> Player;
	UPROPERTY(Transient) TObjectPtr<AWorldMusicManager> MusicManager;

	void EmptyFunction() { CooldownTimer.Invalidate(); }
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	virtual void BeginPlay() override;
};
