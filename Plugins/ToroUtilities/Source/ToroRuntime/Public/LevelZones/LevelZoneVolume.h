// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroVolume.h"
#include "GameplayTagContainer.h"
#include "LevelZoneVolume.generated.h"

UCLASS()
class TORORUNTIME_API ALevelZoneVolume final : public AToroVolume
{
	GENERATED_BODY()

public:
	
	ALevelZoneVolume();

private:

	UPROPERTY(EditAnywhere, Category = Settings)
		FGameplayTag ZoneID;

	UPROPERTY(Transient) TObjectPtr<class AToroGameMode> GameMode;

	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
};
