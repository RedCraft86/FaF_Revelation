// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Quest.h"
#include "ToroDataNode.h"
#include "ToroDataGraph.h"
#include "MetasoundSource.h"
#include "GameplayTagContainer.h"
#include "Actors/TeleportPoint.h"
#include "MiscObjects/UDSSetter.h"
#include "DataTypes/LightingData.h"
#include "Inventory/InventoryAsset.h"
#include "MiscActors/ToroSequenceActor.h"
#include "GamePhaseData.generated.h"

UCLASS()
class FAFREVGAME_API UGamePhaseGraph final : public UToroDataGraph
{
	GENERATED_BODY()

public:

	UGamePhaseGraph();

	// Seconds to wait when stalled before showing widget (Used to not show a widget on short stalls)
	UPROPERTY(EditAnywhere, Category = Loading)
		float WidgetDelay;
};

UCLASS()
class FAFREVGAME_API UGamePhaseNode final : public UToroDataNode
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

	UPROPERTY(EditAnywhere, Category = Game)
		TSoftClassPtr<UQuest> Quest;

	UPROPERTY(EditAnywhere, Category = Game)
		TSoftObjectPtr<UMetaSoundSource> Theme;

	UPROPERTY(EditAnywhere, Category = Game, meta = (Categories = "Content"))
		FGameplayTagContainer UnlockContent;

	UPROPERTY(EditAnywhere, Category = Game, DisplayName = "Sky & Weather")
		FUDSSettings SkyWeather;
	
	UPROPERTY(EditAnywhere, Category = Player)
		TSoftObjectPtr<ATeleportPoint> TeleportPoint;
	
	UPROPERTY(EditAnywhere, Category = Player, meta = (Bitmask, BitmaskEnum = "/Script/FaFRevGame.EPlayerControlFlags"))
		int32 PlayerControl;
	
	UPROPERTY(EditAnywhere, Category = Player)
		FPointLightProperties PlayerLight;

	UPROPERTY(EditAnywhere, Category = "Player|Inventory")
		TSoftObjectPtr<UInventoryAsset> Equipment;

	UPROPERTY(EditAnywhere, Category = "Player|Inventory", meta = (ClampMin = 1, UIMin = 1))
		TMap<TSoftObjectPtr<UInventoryAsset>, uint8> Items;

	UPROPERTY(EditAnywhere, Category = "Player|Inventory")
		TMap<TSoftObjectPtr<UInventoryAsset>, bool> Archives;

	TSet<FGameplayTag> GetContentTags() const;
	TSet<TSoftObjectPtr<UWorld>> GetLevels() const;

	void UnbindSequenceEvents() const;
	void BindSequenceEvents(class UGamePhaseManager* Target) const;
	bool PlayStartSequence() const { return PlaySequence(StartSequence); }
	bool PlayFinishSequence() const { return PlaySequence(FinishSequence); }
	void TeleportPlayer() const { if (TeleportPoint.LoadSynchronous()) TeleportPoint->TeleportPlayer(); }

private:
	static bool PlaySequence(const TSoftObjectPtr<AToroSequenceActor>& InSequence);
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
