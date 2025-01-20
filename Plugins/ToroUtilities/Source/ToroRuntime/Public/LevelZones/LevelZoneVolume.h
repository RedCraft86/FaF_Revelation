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

	UPROPERTY(EditAnywhere, Category = Settings, meta = (Categories = "Zone"))
		FGameplayTag ZoneID;

	UPROPERTY(EditAnywhere, Category = Settings, DisplayName = "Invert")
		bool Cull_Invert;

	/* Actors with Smart Culling Components to render (or not render if inverted) when in volume. */
	UPROPERTY(EditAnywhere, Category = Settings, DisplayName = "Targets")
		TSet<TSoftObjectPtr<AActor>> Cull_Targets;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = Settings, DisplayName = "Bounded")
		bool Cull_FindBounded = true;

	UPROPERTY(EditAnywhere, Category = Settings, DisplayName = "Find Tag")
		FName Cull_FindTag = NAME_None;
#endif
	
	UPROPERTY(EditAnywhere, Category = Settings, DisplayName = "Sound Tag", meta = (Categories = "OneShot"))
		FGameplayTag OneShot_Tag;

	UPROPERTY(EditAnywhere, Category = Settings, DisplayName = "Play Once")
		bool OneShot_PlayOnce;
	
	UPROPERTY(EditAnywhere, Category = Settings, DisplayName = "Cooldown", meta = (EditCondition = "!OneShot_PlayOnce", ClampMin = 1.0f))
		float OneShot_Cooldown;
	
#if WITH_EDITOR
	void FindCullTargets();
#endif
private:

	UPROPERTY() FTimerHandle CullingTimer;
	
	UPROPERTY() bool bCanPlayOneShot;
	UPROPERTY() FTimerHandle OneShotOffTimer;
	UPROPERTY() FTimerHandle OneShotCooldownTimer;
	
	UPROPERTY(Transient) TObjectPtr<class AToroGameMode> GameMode;
	UPROPERTY(Transient) TObjectPtr<class AToroMusicManager> MusicManager;
	UPROPERTY(Transient) TObjectPtr<class AToroPlayerCameraManager> CamManager;

	void UpdateSmartCulling();
	bool CanPlayOneShot() const;
	void PlayOneShot() const;
	void StopOneShot();
	
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
};
