// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/Narrative/Widgets/QuestWidget.h"
#include "Gameplay/Narrative/NarrativeManager.h"
#include "Gameplay/NativeWidgets/NoticeWidget.h"
#include "Components/VerticalBox.h"
#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"

UQuestWidget::UQuestWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), TaskPrefix(INVTEXT("- ")), bVisible(true)
{
	UUserWidget::SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UQuestWidget::ToggleVisibility()
{
	bVisible = !bVisible;
	UpdateFadeState();
}

void UQuestWidget::OnDialogueBegan(UDialogue* Dialogue)
{
	SetHidden(true);
}

void UQuestWidget::OnDialogueFinished(UDialogue* Dialogue, const bool bStartingNewDialogue)
{
	SetHidden(BranchBoxes.IsEmpty());
}

void UQuestWidget::OnQuestNewState(UQuest* Quest, const UQuestState* NewState)
{
	if (NewState) return;
	ClearQuestContainers(Quest);
	for (const UQuestBranch* Branch : NewState->Branches)
	{
		AddOrUpdateQuestBranch(Branch);
	}
	SetHidden(BranchBoxes.IsEmpty());
	if (!BranchBoxes.IsEmpty()) ShowObjectiveNotice();
}

void UQuestWidget::OnQuestTaskCompleted(const UQuest* Quest, const UNarrativeTask* CompletedTask, const UQuestBranch* Branch)
{
	AddOrUpdateQuestBranch(Branch);
	ShowObjectiveNotice();
}

void UQuestWidget::OnQuestTaskProgressChanged(const UQuest* Quest, const UNarrativeTask* ProgressedTask,
	const UQuestBranch* Branch, int32 OldProgress, int32 NewProgress)
{
	AddOrUpdateQuestBranch(Branch);
	ShowObjectiveNotice();
}

void UQuestWidget::OnQuestSucceeded(const UQuest* Quest, const FText& QuestSucceededMessage)
{
	ClearQuestContainers(Quest);
	SetHidden(BranchBoxes.IsEmpty());
}

void UQuestWidget::ShowObjectiveNotice() const
{
	UNoticeWidget::QueueNotice(this, {INVTEXT("Objective Updated")});
}

void UQuestWidget::AddOrUpdateQuestBranch(const UQuestBranch* Branch)
{
	if (!Branch || Branch->bHidden) return;
	if (!Branch->DestinationState || Branch->DestinationState->StateNodeType != EStateNodeType::Failure)
	{
		TArray<FText> Tasks;
		for (const UNarrativeTask* Task : Branch->QuestTasks)
		{
			if (Manager && Task && !Task->bHidden && !Task->IsComplete())
			{
				Tasks.Add(Manager->InjectTextVariables(FText::Format(INVTEXT("{0}{1} {2}"),
					TaskPrefix, Task->GetTaskDescription(), Task->GetTaskProgressText())));
			}
		}
		if (Tasks.IsEmpty()) return;
		if (UVerticalBox* Container = FindOrAddTaskContainer(Branch))
		{
			for (const FText& Task : Tasks)
			{
				if (UTextBlock* TaskText = WidgetTree->ConstructWidget<UTextBlock>())
				{
					Container->AddChild(TaskText);
					TaskText->SetFont(TaskFont);
					TaskText->SetText(Task);
				}
			}
		}
	}
}

UVerticalBox* UQuestWidget::FindOrAddTaskContainer(const UQuestBranch* Branch)
{
	TObjectPtr<UVerticalBox>* Container = BranchBoxes.Find(Branch);
	if (Container && *Container)
	{
		(*Container)->ClearChildren();
		return *Container;
	}

	if (UVerticalBox* NewContainer = WidgetTree->ConstructWidget<UVerticalBox>())
	{
		BranchContainer->AddChild(NewContainer);
		BranchBoxes.Add(Branch, NewContainer);
		return NewContainer;
	}

	return nullptr;
}

void UQuestWidget::ClearQuestContainers(const UQuest* Quest)
{
	for (auto It = BranchBoxes.CreateIterator(); It; ++It)
	{
		if (!It.Key() || It.Key()->GetOwningQuest() == Quest)
		{
			if (It.Value())
			{
				It.Value()->RemoveFromParent();
			}
			It.RemoveCurrent();
		}
	}
}

void UQuestWidget::InitWidget(APlayerController* Controller)
{
	Super::InitWidget(Controller);
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		Manager = UNarrativeManager::Get(this);

		Manager->OnDialogueBegan.AddDynamic(this, &ThisClass::OnDialogueBegan);
		Manager->OnDialogueFinished.AddDynamic(this, &ThisClass::OnDialogueFinished);

		Manager->OnQuestNewState.AddDynamic(this, &ThisClass::OnQuestNewState);
		Manager->OnQuestTaskCompleted.AddDynamic(this, &ThisClass::OnQuestTaskCompleted);
		Manager->OnQuestTaskProgressChanged.AddDynamic(this, &ThisClass::OnQuestTaskProgressChanged);
		Manager->OnQuestSucceeded.AddDynamic(this, &ThisClass::OnQuestSucceeded);
	});
}
