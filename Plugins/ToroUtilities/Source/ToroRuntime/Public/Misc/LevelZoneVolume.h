// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroVolume.h"
#include "NativeGameplayTags.h"
#include "GameplayTagContainer.h"
#include "LevelZoneVolume.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Zone);

inline bool VerifyZoneTag(const FGameplayTag& InTag)
{
	return InTag.IsValid() && InTag != Tag_Zone;
}

UCLASS()
class TORORUNTIME_API ALevelZoneVolume final : public AToroVolume
{
	GENERATED_BODY()

public:
	
	ALevelZoneVolume();

	UPROPERTY(EditAnywhere, Category = Settings, meta = (Categories = "Zone"))
		FGameplayTag ZoneID;

	UPROPERTY(EditAnywhere, Category = Settings, DisplayName = "Invert")
		bool CullInvert;

	/* Actors with Smart Culling Components to render (or not render if inverted) when in volume. */
	UPROPERTY(EditAnywhere, Category = Settings, DisplayName = "Targets")
		TSet<TSoftObjectPtr<AActor>> CullTargets;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = Settings, DisplayName = "Bounded")
		bool CullFindBounded = true;

	UPROPERTY(EditAnywhere, Category = Settings, DisplayName = "Find Tag")
		FName CullFindTag = NAME_None;
#endif
	
	UPROPERTY(EditAnywhere, Category = Settings, DisplayName = "Sound Tag", meta = (Categories = "LocalSounds"))
		FGameplayTag LocalSoundTag;

	UPROPERTY(EditAnywhere, Category = Settings, DisplayName = "Play Once")
		bool LocalSoundPlayOnce;
	
	UPROPERTY(EditAnywhere, Category = Settings, DisplayName = "Cooldown", meta = (EditCondition = "!LocalSound_PlayOnce", ClampMin = 1.0f))
		float LocalSoundCooldown;
	
#if WITH_EDITOR
	void FindCullTargets();
#endif
private:

	UPROPERTY() FTimerHandle CullingTimer;
	
	UPROPERTY() bool bCanPlayLocalSound;
	UPROPERTY() FTimerHandle LocalSoundOffTimer;
	UPROPERTY() FTimerHandle LocalSoundCooldownTimer;
	
	UPROPERTY(Transient) TObjectPtr<class AToroGameMode> GameMode;
	UPROPERTY(Transient) TObjectPtr<class AToroMusicManager> MusicManager;
	UPROPERTY(Transient) TObjectPtr<class AToroPlayerCameraManager> CamManager;

	void UpdateSmartCulling();
	bool CanPlayLocalSound() const;
	void PlayLocalSound() const;
	void StopLocalSound();
	void EmptyFunc();
	
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
};
