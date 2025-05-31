// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Quest.h"
#include "DataNodeBase.h"
#include "DataGraphBase.h"
#include "MetasoundSource.h"
#include "LoadingWidgetBase.h"
#include "Actors/TeleportPoint.h"
#include "DataTypes/LightingData.h"
#include "Interfaces/UDSInterface.h"
#include "Inventory/InventoryItemData.h"
#include "MiscClasses/ToroSequenceActor.h"
#include "GamePhaseData.generated.h"

class UGamePhaseManager;

DECLARE_GAMEPLAY_TAG_BASE(Content)

UCLASS()
class FAFREVGAME_API UGamePhaseGraph final : public UDataGraphBase
{
	GENERATED_BODY()

public:

	UGamePhaseGraph();

	UPROPERTY(EditAnywhere, Category = Loading)
		TSubclassOf<ULoadingWidgetBase> LoadingWidget;

	// Seconds to wait when stalled before showing widget (Used to not show a widget on short stalls)
	UPROPERTY(EditAnywhere, Category = Loading)
		float WidgetDelay;
};

UCLASS()
class FAFREVGAME_API UGamePhaseNode final : public UDataNodeBase
{
	GENERATED_BODY()

public:

	UGamePhaseNode();

	UPROPERTY(EditAnywhere, Category = Level)
		TSoftObjectPtr<UWorld> MainLevel;

	UPROPERTY(EditAnywhere, Category = Level)
		TMap<TSoftObjectPtr<UWorld>, bool> Levels;

	UPROPERTY(EditAnywhere, Category = "Level|Sequences")
		TSoftObjectPtr<AToroSequenceActor> StartSequence;

	UPROPERTY(EditAnywhere, Category = "Level|Sequences")
		TSoftObjectPtr<AToroSequenceActor> FinishSequence;

	UPROPERTY(EditAnywhere, Category = Game, meta = (Base))
		TSoftClassPtr<UQuest> Quest;

	UPROPERTY(EditAnywhere, Category = Game)
		TSoftObjectPtr<UMetaSoundSource> Theme;

	UPROPERTY(EditAnywhere, Category = Game, meta = (Categories = "Content"))
		TSet<FGameplayTag> UnlockContent;

	UPROPERTY(EditAnywhere, Category = Game, DisplayName = "Sky & Weather")
		FUDSSettings SkyWeather;
	
	UPROPERTY(EditAnywhere, Category = Player)
		TSoftObjectPtr<ATeleportPoint> TeleportPoint;
	
	UPROPERTY(EditAnywhere, Category = Player, meta = (Bitmask, BitmaskEnum = "/Script/FaFRevGame.EPlayerControlFlags"))
		int32 PlayerControl;
	
	UPROPERTY(EditAnywhere, Category = Player)
		FPointLightData PlayerLight;

	UPROPERTY(EditAnywhere, Category = "Player|Inventory")
		TSoftObjectPtr<UInventoryItemData> Equipment;

	UPROPERTY(EditAnywhere, Category = "Player|Inventory", meta = (ClampMin = 1, UIMin = 1))
		TMap<TSoftObjectPtr<UInventoryItemData>, uint8> Items;

	UPROPERTY(EditAnywhere, Category = "Player|Inventory")
		TMap<TSoftObjectPtr<UInventoryItemData>, bool> Archives;
	
	TSet<TSoftObjectPtr<UWorld>> GetLevels() const;

	void UnbindSequenceEvents() const;
	void BindSequenceEvents(UGamePhaseManager* Target) const;
	bool PlayStartSequence() const { return PlaySequence(StartSequence); }
	bool PlayFinishSequence() const { return PlaySequence(FinishSequence); }
	void TeleportPlayer() const { if (TeleportPoint.LoadSynchronous()) TeleportPoint->TeleportPlayer(); }

private:
	static bool PlaySequence(const TSoftObjectPtr<AToroSequenceActor>& InSequence);
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};