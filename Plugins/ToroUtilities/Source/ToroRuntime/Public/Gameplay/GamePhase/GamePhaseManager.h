// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

// Fixes AudioMixerQuantizedCommands.h(31): error C2838: 'PlaySoundW': illegal qualified name in member declaration
// for whatever reason. It's actual magic.
#include "Windows/WindowsHWrapper.h"

#include "GamePhaseData.h"
#include "LoadingScreenWidget.h"
#include "Framework/ToroGameState.h"
#include "Components/ActorComponent.h"
#include "Helpers/ClassGetterMacros.h"
#include "GamePhaseManager.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class TORORUNTIME_API UGamePhaseManager final : public UActorComponent
{
	GENERATED_BODY()

public:

	UGamePhaseManager();
	
	GLOBAL_COMPONENT_GETTER(UGamePhaseManager, AToroGameState, GamePhase)

	UFUNCTION(BlueprintCallable, Category = Statics, meta = (WorldContext = "ContextObject"))
	static UGamePhaseManager* GetPhaseManager(const UObject* ContextObject)
	{
		return Get(ContextObject);
	}

	UFUNCTION(BlueprintCallable, Category = GamePhase)
		void LoadSequence();

	UFUNCTION(BlueprintCallable, Category = GamePhase)
		void StepSequence(const uint8 InIndex);

	UFUNCTION(BlueprintCallable, Category = GamePhase)
		void ChangePhase(UGamePhaseNode* NewPhase);

	UFUNCTION(BlueprintPure, Category = GamePhase)
		bool IsLoading() const { return bLoading; }

	// TODO ui

private:

	bool bLoading;
	float PhaseTime;
	uint8 UnloadTasks;
	TArray<uint8> Sequence;
	TSet<TSoftObjectPtr<UWorld>> LoadLevels;
	TSet<TSoftObjectPtr<UWorld>> UnloadLevels;
	
	TObjectPtr<class UNarrativeManager> Narrative;
	TObjectPtr<class UToroSaveManager> SaveManager;
	TObjectPtr<class UWorldMusicManager> MusicManager;
	TObjectPtr<class AToroPlayerCharacter> PlayerChar;
	TObjectPtr<class AMasterPostProcess> PostProcessing;
	
	UPROPERTY(Transient) TObjectPtr<UGamePhaseGraph> Graph;
	UPROPERTY(Transient) TObjectPtr<UGamePhaseNode> ThisPhase;

	TObjectPtr<ULoadingScreenWidget> LoadingWidget;
	ULoadingScreenWidget* GetLoadingWidget();

	bool IsValidManager() const;
	void UnloadLevel(const TSoftObjectPtr<UWorld>& InLevel);
	void LoadLevel(const TSoftObjectPtr<UWorld>& InLevel);
	
	void OnMainLevelLoaded();
	void OnUnloadLevel();

	void FadeToBlack();
	void FadeFromBlack();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
