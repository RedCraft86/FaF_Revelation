// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "Nodes/FlowNode.h"
#include "DataTypes/CachedGetter.h"
#include "MiscActors/TeleportPoint.h"
#include "GameStageNode.generated.h"

class UGameFlagManager;

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameStage);

USTRUCT(BlueprintInternalUseOnly)
struct FStageTeleporter final
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Teleporter)
		TSoftObjectPtr<ATeleportPoint> Target;

	/**
	 * If true, player will only be teleported if this is the first stage loading in.
	 * Otherwise, the player will not teleport for a continous experience.
	 */
	UPROPERTY(EditAnywhere, Category = Teleporter)
		bool bContinuous;

	FStageTeleporter()
		: bContinuous(false)
	{}

	void TeleportTo(const bool bForce) const;
};

UCLASS(NotBlueprintable, BlueprintType)
class UGameStageNode final : public UFlowNode
{
	GENERATED_BODY()

	friend class UGameStageManager;

	static inline const FName InputPinName = TEXT("Enter");
	static inline const FName OutputPinName = TEXT("Complete");

public:

	UGameStageNode();

private:

	UPROPERTY(EditAnywhere, Category = Stage, meta = (Categories = "GameStage"))
		FGameplayTag StageEvent;

	UPROPERTY(EditAnywhere, Category = Stage, meta = (Categories = "Objective"))
		FGameplayTagContainer Objectives;

	UPROPERTY(EditAnywhere, Category = Stage)
		TMap<TSoftObjectPtr<UWorld>, bool> Levels;

	UPROPERTY(EditAnywhere, Category = Stage)
		FStageTeleporter Teleporter;

	UPROPERTY(Transient)
		TWeakObjectPtr<UGameFlagManager> FlagManager;

	UPROPERTY(Transient)
		TSet<FGameplayTag> Requirements;

	void OnFlagUnlocked(const FGameplayTag& Flag);
	virtual void ExecuteInput(const FName& PinName) override;

#if WITH_EDITOR
	virtual FText GetNodeTitle() const override;
	virtual FText GetNodeConfigText() const override;
	virtual EDataValidationResult ValidateNode() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
