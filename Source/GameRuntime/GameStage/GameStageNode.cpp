// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "GameStageNode.h"
#include "GameStageFlow.h"
#include "GameStageManager.h"
#include "GameState/SessionState.h"
#include "GameState/GameFlagManager.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_GameStage, "GameStage")
UE_DEFINE_GAMEPLAY_TAG(TAG_Objective, "Objective")

void FStageTeleporter::TeleportTo(const bool bForce) const
{
	if (bForce || !bContinuous)
	{
		Target->TeleportPlayer();
	}
}

FText GameplayTagToDisplayText(const FGameplayTag& Tag, const FText& Default)
{
	if (Tag.IsValid())
	{
		FString WithoutRoot;
		Tag.ToString().Split(TEXT("."), nullptr, &WithoutRoot);
		if (!WithoutRoot.IsEmpty())
		{
			return FText::FromString(WithoutRoot);
		}
	}

	return Default;
}

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

void UGameStageNode::OnFlagUnlocked(const FGameplayTag& Flag)
{
	Requirements.Remove(Flag);
	if (Requirements.IsEmpty())
	{
		FlagManager->OnFlagUnlocked.RemoveAll(this);
		TriggerOutput(OutputPinName);
	}
}

void UGameStageNode::ExecuteInput(const FName& PinName)
{
	Super::ExecuteInput(PinName);
	if (PinName != InputPinName)
	{
		Super::ExecuteInput(PinName);
		return;
	}

	const ASessionState* State = ASessionState::Get<ASessionState>(this);

	FlagManager = State->GetSessionFlags();
	if (!ensureAlwaysMsgf(FlagManager.IsValid(), TEXT("Failed to get session flag manager. Cannot load game stage.")))
	{
		return;
	}

	for (const FGameplayTag& Objective : Objectives)
	{
		if (!FlagManager->IsFlagUnlocked(Objective))
		{
			Requirements.Add(Objective);
		}
	}

	if (Requirements.IsEmpty())
	{
		TriggerOutput(OutputPinName);
	}
	else
	{
		FlagManager->OnFlagUnlocked.AddUObject(this, &UGameStageNode::OnFlagUnlocked);
		State->GetStageManager()->LoadGameStage(this);
	}
}

#if WITH_EDITOR
FText UGameStageNode::GetNodeTitle() const
{
	return FText::Format(INVTEXT("Stage: {0}"), GameplayTagToDisplayText(StageEvent, INVTEXT("Unknown")));
}

FText UGameStageNode::GetNodeConfigText() const
{
	FTextBuilder TextBuilder;
	TextBuilder.AppendLine(INVTEXT("Objectives:"));
	for (const FGameplayTag& Tag : Objectives)
	{
		TextBuilder.AppendLineFormat(INVTEXT("\t{0}"), 
			GameplayTagToDisplayText(Tag, INVTEXT("Unknown Objective")));
	}
	return TextBuilder.ToText();
}

EDataValidationResult UGameStageNode::ValidateNode()
{
	bool bHasError = false;
	if (!StageEvent.IsValid() || StageEvent == TAG_GameStage.GetTag())
	{
		ValidationLog.Error<UFlowNode>(*FString::Printf(
			TEXT("Invalid Stage Event: %s"), 
			*StageEvent.ToString()), this);

		bHasError = true;
	}

	for (const FGameplayTag& Objective : Objectives)
	{
		if (!Objective.IsValid() || Objective == TAG_Objective.GetTag())
		{
			ValidationLog.Error<UFlowNode>(*FString::Printf(
				TEXT("Invalid Objective Entry: %s"), 
				*Objective.ToString()), this);

			bHasError = true;
		}
	}

	if (Objectives.IsEmpty())
	{
		ValidationLog.Error<UFlowNode>(TEXT("No objective entries! Stages require at least 1."), this);
		bHasError = true;
	}

	bool bHasVisibleLevel = false;
	for (const TPair<TSoftObjectPtr<UWorld>, bool>& Level : Levels)
	{
		if (Level.Key.IsNull())
		{
			ValidationLog.Error<UFlowNode>(TEXT("A level entry is null."), this);
			bHasError = true;
		}
		else if (Level.Value)
		{
			bHasVisibleLevel = true;
		}
	}

	if (!bHasVisibleLevel)
	{
		ValidationLog.Error<UFlowNode>(TEXT("No visible level entries! Stages require at least 1."), this);
		bHasError = true;
	}

	if (Teleporter.Target.IsNull())
	{
		ValidationLog.Error<UFlowNode>(TEXT("Teleporter is null. Stages require a teleporter."), this);
		bHasError = true;
	}

	return bHasError ? EDataValidationResult::Invalid : Super::ValidateNode();
}

void UGameStageNode::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UGameStageNode, StageEvent))
	{
		RequestReconstruction();
	}
}
#endif