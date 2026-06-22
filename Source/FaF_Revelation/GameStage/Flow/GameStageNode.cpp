// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "GameStageNode.h"
#include "GameStageFlow.h"
#include "Framework/ToroGameState.h"
#include "GameStage/FaFGameState.h"
#include "GameStage/GameStageData.h"

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

void UGameStageNode::OnFlagCompleted(const FGameplayTag& Flag)
{
	RemainingTags.Remove(Flag);
	if (RemainingTags.IsEmpty())
	{
		GameSave->OnFlagAdded.RemoveAll(this);
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

	for (const FGameplayTag& Tag : Requirements)
	{
		if (!GameSave->HasFlag(Tag))
		{
			RemainingTags.Add(Tag);
		}
	}

	if (Requirements.IsEmpty())
	{
		// Already completed everything
		TriggerOutput(OutputPinName, true);
	}
	else if (AFaFGameState* GS = AFaFGameState::Get<AFaFGameState>(this))
	{
		GS->LoadGameStage(StageData);
		GameSave->OnFlagAdded.AddUObject(this, &UGameStageNode::OnFlagCompleted);
	}
	else
	{
		UE_LOG(LogGameState, Error, TEXT("Could not find AFaFGameState to load game stage!"))
	}
}

#if WITH_EDITOR
FText UGameStageNode::GetNodeTitle() const
{
	return StageData ? StageData->GetDisplayNameText() : INVTEXT("Game Stage");
}

FText UGameStageNode::GetNodeConfigText() const
{
	FTextBuilder TextBuilder;
	TextBuilder.AppendLine(INVTEXT("Requirements:"));
	for (const FGameplayTag& Tag : Requirements)
	{
		TextBuilder.AppendLineFormat(INVTEXT("\t{0}"), FText::FromString(Tag.ToString()));
	}
	return TextBuilder.ToText();
}

void UGameStageNode::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UGameStageNode, StageData))
	{
		RequestReconstruction();
	}
}
#endif
