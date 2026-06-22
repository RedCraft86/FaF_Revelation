// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "GameStageNode.h"
#include "GameStageFlow.h"
#include "GameStageRequirement.h"
#include "Framework/ToroGameState.h"

UGameStageNode::UGameStageNode()
{
#if WITH_EDITOR
	Category = TEXT("GameStage");
	NodeDisplayStyle = FlowNodeStyle::Latent;
	AllowedAssetClasses = { UGameStageFlow::StaticClass() };
#endif

	InputPins.Reset();
	InputPins.Add(FFlowPin(InputPinName));

	OutputPins.Reset();
	OutputPins.Add(FFlowPin(OutputPinName));
}

EFlowAddOnAcceptResult UGameStageNode::AcceptFlowNodeAddOnChild_Implementation(const UFlowNodeAddOn* AddOnTemplate,
	const TArray<UFlowNodeAddOn*>& AdditionalAddOnsToAssumeAreChildren) const
{
	if (AddOnTemplate && AddOnTemplate->IsA<UGameStageRequirement>())
	{
		return EFlowAddOnAcceptResult::TentativeAccept;
	}

	return Super::AcceptFlowNodeAddOnChild_Implementation(AddOnTemplate, AdditionalAddOnsToAssumeAreChildren);
}

void UGameStageNode::RequirementComplete(const UGameStageRequirement* Requirement)
{
	Requirements.Remove(Requirement);
	if (Requirements.IsEmpty())
	{
		TriggerOutput(OutputPinName, true);
	}
}

void UGameStageNode::ExecuteInput(const FName& PinName)
{
	if (PinName != InputPinName)
	{
		Super::ExecuteInput(PinName);
		return;
	}

	ForEachAddOnForClass<UGameStageRequirement>([this](UFlowNodeAddOn& Addon)
	{
		UGameStageRequirement* Requirement = Cast<UGameStageRequirement>(&Addon);
		if (Requirement->Initialize() && !Requirement->IsFulfilled())
		{
			Requirements.Add(Requirement);
		}

		return EFlowForEachAddOnFunctionReturnValue::Continue;
	});
}
