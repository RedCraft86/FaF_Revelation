// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Actors/ToroVolume.h"
#include "GameplayTagContainer.h"
#include "WorldActions/WorldActionBase.h"
#include "WorldActions/WorldActionComponent.h"
#include "LevelZoneVolume.generated.h"

UCLASS()
class TOROSYSTEMS_API ALevelZoneVolume final : public AToroVolume
{
	GENERATED_BODY()

public:

	ALevelZoneVolume();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UWorldActionComponent> ActionManager;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (Categories = "Zone"))
		FGameplayTag ZoneTag;

	UPROPERTY(EditAnywhere, Category = Settings, DisplayName = "On Enter", NoClear, meta = (ExcludeBaseStruct, HideViewOptions, ShowTreeView))
		TArray<TInstancedStruct<FWorldActionBase>> ActionsEnter;

	UPROPERTY(EditAnywhere, Category = Settings, DisplayName = "On Exit", NoClear, meta = (ExcludeBaseStruct, HideViewOptions, ShowTreeView))
		TArray<TInstancedStruct<FWorldActionBase>> ActionsExit;

	UPROPERTY(EditAnywhere, Category = Settings, DisplayName = "Invert")
		bool CullInvert;
	
	UPROPERTY(EditAnywhere, Category = Settings, DisplayName = "Targets")
		TSet<TSoftObjectPtr<AActor>> CullTargets;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = Tools, DisplayName = "Bounded")
		bool CullFindBounded = true;

	UPROPERTY(EditAnywhere, Category = Tools, DisplayName = "Find Tag")
		FName CullFindTag = NAME_None;

	UFUNCTION() void FindCullTargets();
#endif
private:

	UPROPERTY() FTimerHandle CullingTimer;
	UPROPERTY(Transient) TObjectPtr<APlayerCameraManager> CamManager;

	void UpdateRefCulling();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};
