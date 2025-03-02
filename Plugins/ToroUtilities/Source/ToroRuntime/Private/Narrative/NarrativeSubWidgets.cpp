// Copyright (C) RedCraft86. All Rights Reserved.

#include "Narrative/NarrativeSubWidgets.h"
#include "Narrative/NarrativeWidgetBase.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UQuestTaskWidgetBase::InitWidget(UNarrativeWidgetBase* InParent, const UNarrativeTask* InTask)
{
	ParentUI = InParent;
	TaskObject = InTask;
	if (TaskObject)
	{
		FText TaskDisplay = TaskObject->GetTaskDescription();
		if (!TaskObject->DescriptionOverride.IsEmptyOrWhitespace())
		{
			TaskDisplay = TaskObject->DescriptionOverride;
		}

		if (TaskObject->RequiredQuantity > 1)
		{
			TaskDisplay = FText::FromString(TaskDisplay.ToString() + TEXT(" ") + TaskObject->GetTaskProgressText().ToString());
		}

		DisplayText->SetText(ParentUI->InjectTextVariables(TaskDisplay));
	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
		RemoveFromParent();
	}
}

void UQuestBranchWidgetBase::InitWidget(UNarrativeWidgetBase* InParent, const UQuestBranch* InBranch)
{
	ParentUI = InParent;
	BranchObject = InBranch;
	if (BranchObject)
	{
		DisplayText->SetText(ParentUI->InjectTextVariables(BranchObject->OwningQuest->GetQuestName()));
		DisplayText->SetVisibility(BranchObject->OwningQuest->GetQuestName().IsEmptyOrWhitespace() || !bUseTitle ?
			ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);

		bool bShouldBeHidden = true;
		for (const UNarrativeTask* Task : BranchObject->QuestTasks)
		{
			if (Task && !Task->bHidden && !Task->IsComplete())
			{
				UQuestTaskWidgetBase* TaskWidget = CreateWidget<UQuestTaskWidgetBase>(this, TaskWidgetClass);
				TaskWidget->InitWidget(InParent, Task);

				bShouldBeHidden = false;
				TaskList->AddChild(TaskWidget);
			}
		}

		if (bShouldBeHidden)
		{
			SetVisibility(ESlateVisibility::Collapsed);
			RemoveFromParent();
		}
	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
		RemoveFromParent();
	}
}

UDialogueOptionWidgetBase::UDialogueOptionWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), MaxCharCount(0), SelectedColor(FLinearColor::Gray * FLinearColor::Yellow)
{
#if WITH_EDITOR
	ReplyTaskType = LoadObject<UNarrativeDataTask>(nullptr ,
		TEXT("/Narrative/DefaultTasks/PlayDialogueNode.PlayDialogueNode"));
#endif
}

void UDialogueOptionWidgetBase::OnOptionClicked()
{
	ParentUI->SelectDialogueReply(ReplyObject);
}

void UDialogueOptionWidgetBase::MarkSelected() const
{
	DisplayText->SetColorAndOpacity(SelectedColor);
}

void UDialogueOptionWidgetBase::InitWidget(UNarrativeWidgetBase* InParent, UDialogue* InDialogue, UDialogueNode_Player* InReply)
{
	ParentUI = InParent;
	ReplyObject = InReply;
	if (ParentUI && ReplyObject)
	{
		DisplayText->SetText(ParentUI->InjectTextVariables(ReplyObject->GetOptionText(InDialogue)));
		if (ReplyTaskType && ParentUI->NarrativeComponent->HasCompletedTask(
			ReplyTaskType, ReplyObject->GetID().ToString(), 1))
		{
			DisplayText->SetColorAndOpacity(FLinearColor(0.5f, 0.5f, 0.5f));
		}

		SelectButton->OnClicked.AddDynamic(this, &UDialogueOptionWidgetBase::OnOptionClicked);
		if (DisplayText->GetText().IsEmptyOrWhitespace())
		{
			SetVisibility(ESlateVisibility::Collapsed);
			RemoveFromParent();
		}
	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
		RemoveFromParent();
	}
}
