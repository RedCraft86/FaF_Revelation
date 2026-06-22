// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "GameStageRequirement.h"
#include "GameStageNode.h"

UGameStageRequirement::UGameStageRequirement()
{
#if WITH_EDITOR
	Category = TEXT("GameStage");
	NodeDisplayStyle = FlowNodeStyle::AddOn_Predicate_Composite;
#endif
}

bool UGameStageRequirement::Initialize()
{
	if (!RequiredFlag.IsValid() || SaveObject.IsValid())
	{
		return false;
	}

	SaveObject = UGameSaveObject::Get(this);
	if (!IsFulfilled())
	{
		MessageSystem = UAsyncMessageWorldSubsystem::GetSharedMessageSystem
		   <FAsyncGameplayMessageSystem>(FWorldGetter::Get(this));

		MessageHandle = MessageSystem->BindListener(FAsyncMessageId(RequiredFlag), 
			TWeakObjectPtr(this), &UGameStageRequirement::OnFlagMessage);
	}

	return true;
}

bool UGameStageRequirement::IsFulfilled() const
{
	return SaveObject.IsValid() && SaveObject->HasGameFlag(RequiredFlag);
}

void UGameStageRequirement::OnFlagMessage(const FAsyncMessage& Message)
{
	if (UGameStageNode* Parent = Cast<UGameStageNode>(GetFlowNode()))
	{
		Parent->RequirementComplete(this);
	}

	if (MessageSystem.IsValid())
	{
		MessageSystem->UnbindListener(MessageHandle);
		MessageHandle = FAsyncMessageHandle::Invalid;
	}
}
