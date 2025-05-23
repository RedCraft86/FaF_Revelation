// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Actors/ToroVolume.h"
#include "GameplayTagContainer.h"
#include "LevelZoneVolume.generated.h"

UCLASS()
class TOROUTILITIES_API ALevelZoneVolume final : public AToroVolume
{
	GENERATED_BODY()

public:

	ALevelZoneVolume();

	UPROPERTY(EditAnywhere, Category = Settings, meta = (Categories = "Zone"))
		FGameplayTag ZoneID;

private:

	UPROPERTY() FTimerHandle CullingTimer;
	UPROPERTY(Transient) TObjectPtr<class AToroGameState> GameState;
	UPROPERTY(Transient) TObjectPtr<class AToroCameraManager> CamManager;
	UPROPERTY(Transient) TObjectPtr<class AToroMusicManager> MusicManager;

	void UpdateSmartCulling();
	
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
};
