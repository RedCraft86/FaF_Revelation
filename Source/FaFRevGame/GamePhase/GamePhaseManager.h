// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroSettings.h"
#include "GamePhaseData.h"
#include "Player/GamePlayer.h"
#include "Helpers/ClassGetters.h"
#include "Subsystems/WorldSubsystem.h"
#include "SaveSystem/ToroSaveManager.h"
#include "GamePhaseManager.generated.h"

UCLASS(BlueprintType, DisplayName = "Phase Manager")
class FAFREVGAME_API UGamePhaseManager final : public UWorldSubsystem
{
	GENERATED_BODY()

	friend class UGamePhaseNode;

public:

	UGamePhaseManager(): bLoading(false), bWaiting(false), bMainLoaded(false) {}

	WORLD_SUBSYSTEM_GETTER(UGamePhaseManager);

	UFUNCTION(BlueprintCallable, Category = GamePhase)
		void StepSequence(const uint8 InIndex);

	UFUNCTION(BlueprintCallable, Category = GamePhase)
		void LoadSequence();

	UFUNCTION(BlueprintCallable, Category = GamePhase)
		void ChangePhase(UGamePhaseNode* NewPhase);

	UFUNCTION(BlueprintPure, Category = GamePhase)
		bool IsLoading() const { return bLoading; }

private:

	UPROPERTY() bool bLoading;
	UPROPERTY() bool bWaiting;
	UPROPERTY() bool bMainLoaded;
	UPROPERTY() TArray<uint8> Sequence;
	UPROPERTY() FTimerHandle WidgetTimer;
	UPROPERTY(Transient) TObjectPtr<UGamePhaseGraph> Graph;
	UPROPERTY(Transient) TObjectPtr<ULoadingWidgetBase> LoadingUI;
	UPROPERTY(Transient) TObjectPtr<UToroSaveManager> SaveManager;
	UPROPERTY(Transient) TObjectPtr<AGamePlayer> PlayerChar;

	UPROPERTY(Transient) TObjectPtr<UGamePhaseNode> ThisPhase;
	UPROPERTY(Transient) TSet<TSoftObjectPtr<UWorld>> LevelsToLoad;
	UPROPERTY(Transient) TSet<TSoftObjectPtr<UWorld>> LevelsToUnload;

	void SetLoadingUIHidden(const bool bInHidden) const;
	void ShowLoadUIFunc() const { SetLoadingUIHidden(false); }
	
	void UnloadLevel(const TSoftObjectPtr<UWorld>& InLevel) const;
	void LoadLevel(const TSoftObjectPtr<UWorld>& InLevel);
	void OnMainLevelLoaded();

	void OnStartSequenceEnd();
	void OnFinishSequenceEnd();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override
	{
		return Super::ShouldCreateSubsystem(Outer) && UToroSettings::Get()->IsOnGameplayMap(Outer);
	}

	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override
	{
		return WorldType == EWorldType::Game || WorldType == EWorldType::PIE ||
			WorldType == EWorldType::GamePreview || WorldType == EWorldType::GameRPC;
	}
};
