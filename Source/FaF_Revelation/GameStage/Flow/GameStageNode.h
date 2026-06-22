// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "Nodes/FlowNode.h"
#include "AddOns/FlowNodeAddOn.h"
#include "SaveObjects/GameSaveObject.h"
#include "GameStageNode.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class UGameStageNode final : public UFlowNode
{
	GENERATED_BODY()

	friend class UGameStageRequirement;

	static inline const FName InputPinName = TEXT("Enter");
	static inline const FName OutputPinName = TEXT("Complete");

public:

	UGameStageNode();

private:

	UPROPERTY(Transient)
		TSet<TWeakObjectPtr<UGameStageRequirement>> Requirements;

	virtual EFlowAddOnAcceptResult AcceptFlowNodeAddOnChild_Implementation(const UFlowNodeAddOn* AddOnTemplate, 
		const TArray<UFlowNodeAddOn*>& AdditionalAddOnsToAssumeAreChildren) const override;

	void RequirementComplete(const UGameStageRequirement* Requirement);
	virtual void ExecuteInput(const FName& PinName) override;

#if WITH_EDITOR
	virtual FText GetNodeTitle() const override { return INVTEXT("Game Stage"); }
#endif
};
