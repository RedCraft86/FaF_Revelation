// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Actors/ToroVolume.h"
#include "Helpers/GameplayTagHelpers.h"
#include "ZoneVolume.generated.h"

DECLARE_GAMEPLAY_TAG_BASE(Zone);

UCLASS()
class TOROUTILITIES_API AZoneVolume final : public AToroVolume
{
	GENERATED_BODY()

public:

	AZoneVolume();

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (Categories = "Zone"))
		FGameplayTag ZoneID;

	UPROPERTY(EditAnywhere, Category = "Settings|Culling", DisplayName = "Invert")
		bool CullInvert;
	
	UPROPERTY(EditAnywhere, Category = "Settings|Culling", DisplayName = "Targets")
		TSet<TSoftObjectPtr<AActor>> CullTargets;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = Tools, DisplayName = "Bounded")
		bool CullFindBounded = true;

	UPROPERTY(EditAnywhere, Category = Tools, DisplayName = "Find Tag")
		FName CullFindTag = NAME_None;
	
	UFUNCTION(CallInEditor, Category = Tools)
		void FindCullTargets();
#endif
private:

	UPROPERTY() FTimerHandle CullingTimer;
	UPROPERTY(Transient) TObjectPtr<class AToroGameState> GameState;
	UPROPERTY(Transient) TObjectPtr<class AToroCameraManager> CamManager;

	void UpdateSmartCulling();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
};
