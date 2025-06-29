// Copyright (C) RedCraft86. All Rights Reserved.

#include "Narrative/NarrativeSubWidgets.h"
#include "Narrative/NarrativeWidget.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "QuestTask.h"
#include "QuestSM.h"
#include "Quest.h"

void UQuestTaskWidget::InitWidget(UQuestWidget* InParent, const UNarrativeTask* InTask)
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
			TaskDisplay = FText::Format(INVTEXT("{0} {1}"), TaskDisplay, TaskObject->GetTaskProgressText());
		}

		DisplayText->SetText(ParentUI->InjectTextVariables(TaskDisplay));
	}
	else
	{
		RemoveFromParent();
	}
}

void UQuestBranchWidget::InitWidget(UQuestWidget* InParent, const UQuestBranch* InBranch)
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
				UQuestTaskWidget* TaskWidget = CreateWidget<UQuestTaskWidget>(this, TaskWidgetClass);
				TaskWidget->InitWidget(InParent, Task);

				bShouldBeHidden = false;
				TaskList->AddChild(TaskWidget);
			}
		}

		if (bShouldBeHidden)
		{
			RemoveFromParent();
		}
	}
	else
	{
		RemoveFromParent();
	}
}

UDialogueOptionWidget::UDialogueOptionWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), MaxCharCount(0), SelectedColor(FLinearColor::Gray * FLinearColor::Yellow)
{}

void UDialogueOptionWidget::OnOptionClicked()
{
	ParentUI->SelectReplyOption(ReplyObject);
}

void UDialogueOptionWidget::MarkSelected() const
{
	DisplayText->SetColorAndOpacity(SelectedColor);
}

void UDialogueOptionWidget::InitWidget(UDialogueWidget* InParent, UDialogue* InDialogue, UDialogueNode_Player* InReply)
{
	ParentUI = InParent;
	ReplyObject = InReply;
	if (ParentUI && ReplyObject)
	{
		DisplayText->SetText(ParentUI->InjectTextVariables(ReplyObject->GetOptionText(InDialogue)));
		if (ParentUI->HasChosenOption(InReply))
		{
			DisplayText->SetColorAndOpacity(FLinearColor(0.5f, 0.5f, 0.5f));
		}

		SelectButton->OnClicked.AddDynamic(this, &UDialogueOptionWidget::OnOptionClicked);
		if (DisplayText->GetText().IsEmptyOrWhitespace())
		{
			RemoveFromParent();
		}
	}
	else
	{
		RemoveFromParent();
	}
}
