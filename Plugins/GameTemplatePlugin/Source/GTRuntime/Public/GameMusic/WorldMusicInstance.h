// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "MusicManager.h"
#include "GameFramework/Actor.h"
#include "WorldMusicInstance.generated.h"

UCLASS(NotBlueprintable)
class GTRUNTIME_API AWorldMusicInstance : public ABrush
{
	GENERATED_BODY()

public:

	AWorldMusicInstance();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Subobjects")
		TObjectPtr<UAudioComponent> AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		EMusicChannel Channel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		TObjectPtr<USoundBase> Sound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
		float Volume = 1.0f;

protected:
	
	UPROPERTY() bool bMuted = false;
	UPROPERTY() bool bOverlapping = false;
	
	UPROPERTY(Transient)
		TObjectPtr<AMusicManager> MusicManager;

	void SetMuted(const bool bInMuted, const bool bImmediate = false);
	void OnChannelUpdate(const EMusicChannel InChannel, const bool bInMuted, const bool bImmediate);
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	virtual void BeginPlay() override;
};
