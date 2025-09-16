// Copyright (C) RedCraft86. All Rights Reserved.

#include "Narrative/NarrativeWidget.h"
#include "Framework/ToroWidgetManager.h"
#include "UserWidgets/MessagingWidget.h"
#include "UserWidgets/ExprTextBlock.h"
#include "Narrative/ToroNarrative.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

UQuestWidget::UQuestWidget(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	ZOrder = 94;
}

void UQuestWidget::QuestUpdatedNotify() const
{
	UMessagingWidget::QueueNotice(this, {INVTEXT("Objective Updated"), 1.0f});
}

void UQuestWidget::RefreshQuestList(const UQuest* Quest, const UQuestBranch* Branch)
{
	BranchBox->ClearChildren();
	if (Branch && (!Branch->DestinationState || Branch->DestinationState->StateNodeType != EStateNodeType::Failure))
	{
		if (!Branch->DestinationState || Branch->DestinationState->StateNodeType != EStateNodeType::Failure)
		{
			UQuestBranchWidget* BranchWidget = CreateWidget<UQuestBranchWidget>(this, BranchWidgetClass);
			BranchWidget->InitWidget(this, Branch);

			BranchBox->AddChild(BranchWidget);
		}

		SetHidden(false);
	}
}

void UQuestWidget::OnDialogueBegan(UDialogue* Dialogue)
{
	SetHidden(true);
}

void UQuestWidget::OnDialogueFinished(UDialogue* Dialogue, const bool bStartingNewDialogue)
{
	SetHidden(BranchBox->HasAnyChildren());
}

void UQuestWidget::OnQuestNewState(UQuest* Quest, const UQuestState* NewState)
{
	GetWorld()->GetTimerManager().SetTimerForNextTick([this, NewState]()
	{
		if (NewState && !NewState->Branches.IsEmpty())
		{
			BranchBox->ClearChildren();
			for (const UQuestBranch* Branch : NewState->Branches)
			{
				if (!Branch || Branch->bHidden) continue;
				UQuestBranchWidget* BranchWidget = CreateWidget<UQuestBranchWidget>(this, BranchWidgetClass);
				BranchWidget->InitWidget(this, Branch);
				BranchBox->AddChild(BranchWidget);
			}

			SetHidden(false);
		}
	});
}

void UQuestWidget::OnQuestTaskCompleted(const UQuest* Quest,
	const UNarrativeTask* CompletedTask, const UQuestBranch* Branch)
{
	if (Branch && Branch->DestinationState && Branch->DestinationState->StateNodeType != EStateNodeType::Failure)
	{
		RefreshQuestList(Quest, Branch);
	}
}

void UQuestWidget::OnQuestTaskProgressChanged(const UQuest* Quest, const UNarrativeTask* ProgressedTask,
	const UQuestBranch* Branch, int32 OldProgress, int32 NewProgress)
{
	if (Branch && ProgressedTask && !ProgressedTask->IsComplete() && (!Branch->DestinationState
		|| Branch->DestinationState->StateNodeType != EStateNodeType::Failure))
	{
		RefreshQuestList(Quest, Branch);
	}
}

void UQuestWidget::OnQuestSucceeded(const UQuest* Quest, const FText& QuestSucceededMessage)
{
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, [this](){
		SetHidden(true);
	}, 0.6f, false);
}

void UQuestWidget::InitWidget()
{
	Super::InitWidget();
	Narrative = UToroNarrative::Get(this);

	Narrative->OnDialogueBegan.AddDynamic(this, &ThisClass::OnDialogueBegan);
	Narrative->OnDialogueFinished.AddDynamic(this, &ThisClass::OnDialogueFinished);

	Narrative->OnQuestNewState.AddDynamic(this, &ThisClass::OnQuestNewState);
	Narrative->OnQuestTaskCompleted.AddDynamic(this, &ThisClass::OnQuestTaskCompleted);
	Narrative->OnQuestTaskProgressChanged.AddDynamic(this, &ThisClass::OnQuestTaskProgressChanged);
	Narrative->OnQuestSucceeded.AddDynamic(this, &ThisClass::OnQuestSucceeded);
}

UDialogueWidget::UDialogueWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), PlayerColor(FLinearColor::Yellow), NPCColor(FLinearColor::White)
{
	ZOrder = 96;
	bAutoActivate = false;

	ReplyTaskType = LoadObject<UNarrativeDataTask>(nullptr ,
		TEXT("/Narrative/DefaultTasks/PlayDialogueNode.PlayDialogueNode"));
}

bool UDialogueWidget::HasChosenOption(const UDialogueNode_Player* InChoice) const
{
	return InChoice && Narrative->HasCompletedTask(ReplyTaskType, InChoice->GetID().ToString(), 1);
}

void UDialogueWidget::SelectReplyOption(UDialogueNode_Player* InChoice)
{
	Narrative->SelectDialogueOption(InChoice);
	for (const TPair<FName, TObjectPtr<UDialogueOptionWidget>>& DialogueReply : Replies)
	{
		if (!DialogueReply.Value) continue;
		DialogueReply.Value->SetColorAndOpacity(FLinearColor::Gray);
		DialogueReply.Value->DisplayText->SetColorAndOpacity(FLinearColor::White);
		DialogueReply.Value->SetVisibility(ESlateVisibility::HitTestInvisible);
		if (DialogueReply.Key == InChoice->GetID())
		{
			DialogueReply.Value->MarkSelected();
		}
	}

	SkipButton->SetIsEnabled(true);
	SkipButton->SetRenderOpacity(1.0f);
	PlayAnimation(RepliesAnim);
}

void UDialogueWidget::OnSkipClicked()
{
	Narrative->SkipCurrentDialogueLine();
}

void UDialogueWidget::OnBegan(UDialogue* Dialogue)
{
	if (SimpleSubtitles) SimpleSubtitles->SetHidden(true);
}

void UDialogueWidget::OnFinished(UDialogue* Dialogue, const bool bStartingNewDialogue)
{
	ReplyBox->ClearChildren();
	PlayAnimationReverse(RepliesAnim, 10.0f);
	if (SimpleSubtitles) SimpleSubtitles->SetHidden(false);
	DeactivateWidget();
}

void UDialogueWidget::OnRepliesAvailable(UDialogue* Dialogue, const TArray<UDialogueNode_Player*>& PlayerReplies)
{
	ReplyBox->ClearChildren();
	if (PlayerReplies.Num() == 1)
	{
		Narrative->SelectDialogueOption(PlayerReplies[0]);
		return;
	}

	SkipButton->SetIsEnabled(false);
	SkipButton->SetRenderOpacity(0.25f);
	for (UDialogueNode_Player* Reply : PlayerReplies)
	{
		if (!Reply) continue;
		UDialogueOptionWidget* Widget = CreateWidget<UDialogueOptionWidget>(this, ReplyWidgetClass);
		Widget->SetPadding(ReplyPadding);
		Widget->InitWidget(this, Dialogue, Reply);

		Replies.Add(Reply->GetID(), Widget);
		ReplyBox->AddChild(Widget);
	}
	
	PlayAnimation(RepliesAnim);
}

void UDialogueWidget::OnPlayerLineStarted(UDialogue* Dialogue, UDialogueNode_Player* Node, const FDialogueLine& Line)
{
	SpeakerText->SetText(INVTEXT("You"));
	SpeakerText->SetColorAndOpacity(PlayerColor);
	TitleText->SetVisibility(ESlateVisibility::Collapsed);
	LineText->SetText(InjectTextVariables(Line.Text));
	PlayAnimation(ActivateAnim);
}

void UDialogueWidget::OnPlayerLineFinished(UDialogue* Dialogue, UDialogueNode_Player* Node, const FDialogueLine& Line)
{
	PlayAnimationReverse(RepliesAnim);
}

void UDialogueWidget::OnNPCLineStarted(UDialogue* Dialogue, UDialogueNode_NPC* Node, const FDialogueLine& Line, const FSpeakerInfo& Speaker)
{
	SpeakerText->SetText(FText::FromName(Speaker.SpeakerID));
	SpeakerText->SetColorAndOpacity(NPCColor);

	TitleText->SetText(Speaker.SpeakerName);
	TitleText->SetVisibility(Speaker.SpeakerName.IsEmptyOrWhitespace()
		? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);

	LineText->SetText(InjectTextVariables(Line.Text));
	PlayAnimation(ActivateAnim);
}

void UDialogueWidget::InitWidget()
{
	Super::InitWidget();
	Narrative = UToroNarrative::Get(this);

	Narrative->OnDialogueBegan.AddDynamic(this, &ThisClass::OnBegan);
	Narrative->OnDialogueFinished.AddDynamic(this, &ThisClass::OnFinished);
	Narrative->OnDialogueRepliesAvailable.AddDynamic(this, &ThisClass::OnRepliesAvailable);
	Narrative->OnPlayerDialogueLineStarted.AddDynamic(this, &ThisClass::OnPlayerLineStarted);
	Narrative->OnPlayerDialogueLineFinished.AddDynamic(this, &ThisClass::OnPlayerLineFinished);
	Narrative->OnNPCDialogueLineStarted.AddDynamic(this, &ThisClass::OnNPCLineStarted);

	SkipButton->OnClicked.AddDynamic(this, &ThisClass::OnSkipClicked);
}

void UDialogueWidget::InternalProcessActivation()
{
	Super::InternalProcessActivation();
	if (!SimpleSubtitles)
	{
		if (AToroWidgetManager* Manager = AToroWidgetManager::Get(this))
		{
			SimpleSubtitles = Manager->FindWidget<USubtitleWidget>();
		}
	}
}
