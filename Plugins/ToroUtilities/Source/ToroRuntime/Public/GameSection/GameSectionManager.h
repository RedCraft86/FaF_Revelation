// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameSectionGraph.h"
#include "LoadingWidgetBase.h"
#include "ClassGetterHelpers.h"
#include "Subsystems/WorldSubsystem.h"
#include "SaveSystem/BaseSaveObjects.h"
#include "GameSectionManager.generated.h"

UCLASS(BlueprintType, DisplayName = "Game Section")
class TORORUNTIME_API UGameSectionManager final : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	UGameSectionManager() : bLoading(false), bWaiting(false), bMainLevelLoaded(false) {}
	WORLD_SUBSYSTEM_GETTER(UGameSectionManager);

	UFUNCTION(BlueprintCallable, Category = GameSection, meta = (SaveTag = "Saves.Game"))
		void StepSequence(const uint8 InIndex, UPARAM(meta = (Categories = "Saves")) const FGameplayTag SaveTag);

	UFUNCTION(BlueprintCallable, Category = GameSection, meta = (SaveTag = "Saves.Game"))
		void LoadSequence(UPARAM(meta = (Categories = "Saves")) const FGameplayTag SaveTag);

	UFUNCTION(BlueprintCallable, Category = GameSection, meta = (SaveTag = "Saves.Game"))
		void ChangeSection(UGameSectionNode* NewSection, UPARAM(meta = (Categories = "Saves")) const FGameplayTag SaveTag);

	UFUNCTION(BlueprintPure, Category = GameSection)
		bool IsLoading() const { return bLoading; }
	
private:

	static inline uint8 LatentID = 255;
	static uint8 GetLatentID()
	{
		if (LatentID == 0) LatentID = 255;
		return LatentID--;
	}

	UPROPERTY() bool bLoading;
	UPROPERTY() bool bWaiting;
	UPROPERTY() bool bMainLevelLoaded;
	UPROPERTY() FTimerHandle WidgetTimer;
	UPROPERTY() TSet<TSoftObjectPtr<UWorld>> ToLoad;
	UPROPERTY() TSet<TSoftObjectPtr<UWorld>> ToUnload;
	UPROPERTY() TObjectPtr<ULoadingWidgetBase> Widget;

	UPROPERTY() TArray<uint8> Sequence;
	UPROPERTY() TObjectPtr<AActor> UltraDynamicSky;
	UPROPERTY() TObjectPtr<UGameSectionNode> Section;
	UPROPERTY() TObjectPtr<UGameSectionGraph> Graph;
	UPROPERTY() TObjectPtr<UGlobalSaveObjectBase> GlobalSave;
	UPROPERTY() TObjectPtr<UGameSaveObjectBase> GameSave;
	UPROPERTY() FGameplayTag LastSaveTag;

	void SetWidgetHidden(const bool bInHidden);
	void DelayedShowWidget() { SetWidgetHidden(false); }

	void UnloadLevel(const TSoftObjectPtr<UWorld>& InLevel) const;
	void LoadLevel(const TSoftObjectPtr<UWorld>& InLevel);
	void OnMainLevelLoaded();

	void OnStartSequenceFinished();
	void OnEndSequenceFinished();

	UGlobalSaveObjectBase* GetGlobalSave();
	UGameSaveObjectBase* GetGameSave(const FGameplayTag& SaveTag);
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
};
