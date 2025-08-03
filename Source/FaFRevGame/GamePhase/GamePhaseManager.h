// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

// Fixes AudioMixerQuantizedCommands.h(31): error C2838: 'PlaySoundW': illegal qualified name in member declaration
// for whatever reason. It's actual magic.
#include "Windows/WindowsHWrapper.h"

#include "GamePhaseData.h"
#include "ToroUtilities.h"
#include "GameLoadingWidget.h"
#include "GameHints/HintSubsystem.h"
#include "Subsystems/WorldSubsystem.h"
#include "PlayerChar/GamePlayerChar.h"
#include "SaveSystem/ToroSaveManager.h"
#include "GamePhaseManager.generated.h"

UCLASS()
class FAFREVGAME_API UGamePhaseManager final : public UTickableWorldSubsystem
{
	GENERATED_BODY()

	friend class UGamePhaseNode;

public:

	UGamePhaseManager(): bLoading(false), bWaiting(false), bMainLoad(false), PhaseTime(0.0f), UnloadTasks(0) {}

	WORLD_SUBSYSTEM_GETTER(UGamePhaseManager);

	UFUNCTION(BlueprintCallable, Category = GamePhase)
		void LoadSequence();

	UFUNCTION(BlueprintCallable, Category = GamePhase)
		void StepSequence(const uint8 InIndex);

	UFUNCTION(BlueprintCallable, Category = GamePhase)
		void ChangePhase(UGamePhaseNode* NewPhase);

	UFUNCTION(BlueprintPure, Category = GamePhase)
		bool IsLoading() const { return bLoading; }

private:

	UPROPERTY() bool bLoading;
	UPROPERTY() bool bWaiting;
	UPROPERTY() bool bMainLoad;
	UPROPERTY() float PhaseTime;
	UPROPERTY() uint8 UnloadTasks;
	UPROPERTY() TArray<uint8> Sequence;
	UPROPERTY() FTimerHandle WidgetTimer;
	UPROPERTY(Transient) TSet<TSoftObjectPtr<UWorld>> ToLoad;
	UPROPERTY(Transient) TSet<TSoftObjectPtr<UWorld>> ToUnload;
	UPROPERTY(Transient) TObjectPtr<UGamePhaseNode> ThisPhase;

	UPROPERTY(Transient) TObjectPtr<UGamePhaseGraph> Graph;
	UPROPERTY(Transient) TObjectPtr<UHintSubsystem> HintSystem;
	UPROPERTY(Transient) TObjectPtr<AGamePlayerChar> PlayerChar;
	UPROPERTY(Transient) TObjectPtr<UGameLoadingWidget> LoadingUI;

	void ShowLoadUI() const { if (LoadingUI) LoadingUI->ActivateWidget(); }
	void HideLoadUI() const { if (LoadingUI) LoadingUI->DeactivateWidget(); }

	void UnloadLevels();
	void OnUnloadLevel();
	void LoadLevel(const TSoftObjectPtr<UWorld>& InLevel);
	void OnMainLevelLoaded();

	void OnStartSequenceEnd();
	void OnFinishSequenceEnd();

	virtual void Tick(float DeltaTime) override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual TStatId GetStatId() const override { return GetStatID(); }
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override
	{
		return Super::ShouldCreateSubsystem(Outer) && UToroUtilSettings::Get()->IsOnGameplayMap(Outer);
	}
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override
	{
		return WorldType == EWorldType::Game || WorldType == EWorldType::PIE ||
			WorldType == EWorldType::GamePreview || WorldType == EWorldType::GameRPC;
	}
};
