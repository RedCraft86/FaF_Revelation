// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "Nodes/FlowNode.h"
#include "AddOns/FlowNodeAddOn.h"
#include "SaveObjects/GameSaveObject.h"
#include "GameStageNode.generated.h"

class UGameStageData;

UCLASS(NotBlueprintable, BlueprintType)
class UGameStageNode final : public UFlowNode
{
	GENERATED_BODY()

	friend class FGameStageNodeDetails;

	static inline const FName InputPinName = TEXT("Enter");
	static inline const FName OutputPinName = TEXT("Complete");

public:

	UGameStageNode();

private:

	UPROPERTY(EditAnywhere, Category = Stage, meta = (Categories = "Objective"))
		FGameplayTagContainer Requirements;

	UPROPERTY(EditAnywhere, Category = Stage)
		TObjectPtr<UGameStageData> StageData;

	UPROPERTY(Transient)
		TSet<FGameplayTag> RemainingTags;

	UPROPERTY(Transient)
		TWeakObjectPtr<UGameSaveObject> GameSave;

	void OnFlagCompleted(const FGameplayTag& Flag);
	virtual void ExecuteInput(const FName& PinName) override;

#if WITH_EDITOR
	virtual FText GetNodeConfigText() const override;
	virtual FText GetNodeTitle() const override { return INVTEXT("Game Stage"); }
#endif
};
