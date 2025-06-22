// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Actors/ToroVolume.h"
#include "LocalMusicVolume.generated.h"

UCLASS()
class TOROSYSTEMS_API ALocalMusicVolume : public AToroVolume
{
	GENERATED_BODY()

public:

	ALocalMusicVolume();

	UPROPERTY(EditAnywhere, Category = Settings)
		TObjectPtr<USoundBase> Sound;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClmapMin = 0.0f, ClampMax = 0.0f))
		float Cooldown;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClmapMin = 0.1f, ClampMax = 0.1f))
		float Volume;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClmapMin = 0.0f, ClampMax = 0.0f))
		FVector2D StartRange;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClmapMin = 0.1f, ClampMax = 0.1f))
		FVector2D FadeInOut;

	UPROPERTY(EditAnywhere, Category = Settings)
		bool bDipMainTheme;

	UPROPERTY(EditAnywhere, Category = Settings)
		TSet<uint8> AllowedStates;

private:

	UPROPERTY() bool bInVolume;
	UPROPERTY() bool bOnCooldown;
	UPROPERTY() uint8 MusicState;
	UPROPERTY(Transient) TObjectPtr<class UGameMusicManager> Manager;

	UPROPERTY(VisibleAnywhere, Category = Subobjects)
		TObjectPtr<UAudioComponent> SoundComponent;

	void CooldownFinish()
	{
		bOnCooldown = false;
		UpdateAudio();
	}
	void StopUntilNextTrigger()
	{
		bInVolume = false;
		UpdateAudio();
	}
	void OnMusicStateChanged(const uint8 State)
	{
		MusicState = State;
		UpdateAudio();
	}

	void UpdateAudio();
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
};
