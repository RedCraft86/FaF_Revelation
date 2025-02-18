// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "SectionLoadWidget.h"
#include "GameFramework/Actor.h"
#include "Misc/ToroSequenceActor.h"
#include "LevelTransitionActor.generated.h"

UCLASS(NotBlueprintable, meta = (HiddenCategories = "Rendering"))
class TORORUNTIME_API ALevelTransitionActor final : public AActor
{
	GENERATED_BODY()

public:

	ALevelTransitionActor();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

#if WITH_EDITORONLY_DATA
	UPROPERTY() TObjectPtr<UBillboardComponent> DebugBillboard;
#endif
	
	UPROPERTY(EditAnywhere, Category = Settings)
		TSoftObjectPtr<class ATeleportTargetActor> Teleport;
	
	UPROPERTY(EditAnywhere, Category = Settings)
		TSoftObjectPtr<UWorld> MainLevel;

	UPROPERTY(EditAnywhere, Category = Settings)
		TMap<TSoftObjectPtr<UWorld>, bool> Levels;

	UPROPERTY(EditAnywhere, Category = "Settings|Sequences")
		TSoftObjectPtr<AToroSequenceActor> StartSequence;

	UPROPERTY(EditAnywhere, Category = "Settings|Sequences")
		TSoftObjectPtr<AToroSequenceActor> WaitSequence;

	UPROPERTY(EditAnywhere, Category = "Settings|Sequences")
		TSoftObjectPtr<AToroSequenceActor> EndSequence;


	TSet<TSoftObjectPtr<UWorld>> BeginTransition(const TSet<TSoftObjectPtr<UWorld>>& OldLevels);

private:

	static inline uint8 LatentID = 0;
	static uint8 GetLatentID() { return LatentID++; }
	
	UPROPERTY() bool bWaiting;
	UPROPERTY() bool bMainLevelLoaded;
	UPROPERTY() TSet<TSoftObjectPtr<UWorld>> ToLoad;
	UPROPERTY() TSet<TSoftObjectPtr<UWorld>> ToUnload;
	UPROPERTY() TObjectPtr<USectionLoadWidget> Widget;

	void SetWidgetHidden(const bool bInHidden);

	void UnloadLevel(const TSoftObjectPtr<UWorld>& InLevel) const;
	void LoadLevel(const TSoftObjectPtr<UWorld>& InLevel);
	void OnMainLevelLoaded();

	void OnStartSequenceFinished();
	void OnEndSequenceFinished();

	virtual void BeginPlay() override;
#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};
