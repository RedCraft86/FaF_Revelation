// Copyright (C) RedCraft86. All Rights Reserved.

#include "NarrativeWidgetBase.h"
#include "Framework/ToroWidgetManager.h"
#include "UserWidgets/ExprTextBlock.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Misc/ToroNarrative.h"
#include "MessageWidgetBase.h"

void UNarrativeWidgetBase::SetQuestsHidden(const bool bInHidden)
{
	if (bHideQuests != bInHidden && !IsAnimationPlaying(QuestFadeAnim))
	{
		bHideQuests = bInHidden;
		bHideQuests ? PlayAnimationForward(QuestFadeAnim) : PlayAnimationReverse(QuestFadeAnim);
	}
}

void UNarrativeWidgetBase::QuestUpdatedNotify() const
{
	if (bNotifyObjectives)
	{
		UMessageWidgetBase::QueueSmallNotice(this,
			{INVTEXT("Objective Updated"), 1.0f}, false);
	}
}

void UNarrativeWidgetBase::RefreshQuestList(const UQuest* Quest, const UQuestBranch* Branch)
{
	QuestBranchBox->ClearChildren();
	if (Branch && (!Branch->DestinationState || Branch->DestinationState->StateNodeType != EStateNodeType::Failure))
	{
		if (!Branch->DestinationState || Branch->DestinationState->StateNodeType != EStateNodeType::Failure)
		{
			UQuestBranchWidgetBase* BranchWidget = CreateWidget<UQuestBranchWidgetBase>(this, BranchWidgetClass);
			BranchWidget->InitWidget(this, Branch);

			QuestBranchBox->AddChild(BranchWidget);
		}

		bHideQuests = false;
		QuestUpdatedNotify();
		PlayAnimation(QuestFadeAnim, 0.0f, 1, EUMGSequencePlayMode::Reverse);
	}
}

void UNarrativeWidgetBase::OnQuestNewState(UQuest* Quest, const UQuestState* NewState)
{
	GetWorld()->GetTimerManager().SetTimerForNextTick([this, NewState]()
	{
		if (NewState && !NewState->Branches.IsEmpty())
		{
			QuestBranchBox->ClearChildren();
			for (const UQuestBranch* Branch : NewState->Branches)
			{
				if (Branch && !Branch->bHidden)
				{
					UQuestBranchWidgetBase* BranchWidget = CreateWidget<UQuestBranchWidgetBase>(this, BranchWidgetClass);
					BranchWidget->InitWidget(this, Branch);

					QuestBranchBox->AddChild(BranchWidget);
				}
			}

			bHideQuests = false;
			QuestUpdatedNotify();
			PlayAnimation(QuestFadeAnim, 0.0f, 1, EUMGSequencePlayMode::Reverse);
		}
	});
}

void UNarrativeWidgetBase::OnQuestTaskCompleted(const UQuest* Quest, const UNarrativeTask* CompletedTask, const UQuestBranch* Branch)
{
	if (Branch && Branch->DestinationState && Branch->DestinationState->StateNodeType != EStateNodeType::Failure)
	{
		RefreshQuestList(Quest, Branch);
	}
}

void UNarrativeWidgetBase::OnQuestTaskProgressChanged(const UQuest* Quest, const UNarrativeTask* ProgressedTask, const UQuestBranch* Branch, int32 OldProgress, int32 NewProgress)
{
	if (Branch && ProgressedTask && !ProgressedTask->IsComplete() && (!Branch->DestinationState
		|| Branch->DestinationState->StateNodeType != EStateNodeType::Failure))
	{
		RefreshQuestList(Quest, Branch);
	}
}

void UNarrativeWidgetBase::OnQuestSucceeded(const UQuest* Quest, const FText& QuestSucceededMessage)
{
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, [this](){
		SetQuestsHidden(true);
	}, 0.6f, false);
}

void UNarrativeWidgetBase::SelectDialogueReply(UDialogueNode_Player* Reply)
{
	NarrativeComponent->SelectDialogueOption(Reply);
	for (const TPair<FName, TObjectPtr<UDialogueOptionWidgetBase>>& DialogueReply : DialogueReplies)
	{
		if (!DialogueReply.Value) continue;
		DialogueReply.Value->SetColorAndOpacity(FLinearColor::Gray);
		DialogueReply.Value->DisplayText->SetColorAndOpacity(FLinearColor::White);
		DialogueReply.Value->SetVisibility(ESlateVisibility::HitTestInvisible);
		if (DialogueReply.Key == Reply->GetID())
		{
			DialogueReply.Value->MarkSelected();
		}
	}

	SkipLineButton->SetIsEnabled(true);
	SkipLineButton->SetRenderOpacity(1.0f);
	PlayAnimation(RepliesFadeAnim);
}

void UNarrativeWidgetBase::OnDialogueBegan(UDialogue* Dialogue)
{
	SetQuestsHidden(true);
}

void UNarrativeWidgetBase::OnDialogueFinished(UDialogue* Dialogue, const bool bStartingNewDialogue)
{
	DialogueReplyBox->ClearChildren();
	PlayAnimationReverse(DialogueFadeAnim);
	if (GetAnimationCurrentTime(RepliesFadeAnim) > 0.0f)
	{
		PlayAnimationReverse(RepliesFadeAnim);
	}
}

void UNarrativeWidgetBase::OnDialogueRepliesAvailable(UDialogue* Dialogue, const TArray<UDialogueNode_Player*>& PlayerReplies)
{
	DialogueReplyBox->ClearChildren();
	if (PlayerReplies.Num() == 1)
	{
		NarrativeComponent->SelectDialogueOption(PlayerReplies[0]);
		return;
	}

	SkipLineButton->SetIsEnabled(false);
	SkipLineButton->SetRenderOpacity(0.5f);
	for (UDialogueNode_Player* Reply : PlayerReplies)
	{
		if (!Reply) continue;
		UDialogueOptionWidgetBase* Widget = CreateWidget<UDialogueOptionWidgetBase>(this, ReplyWidgetClass);
		Widget->SetPadding(DialogueReplyPadding);
		Widget->InitWidget(this, Dialogue, Reply);

		DialogueReplies.Add(Reply->GetID(), Widget);
		DialogueReplyBox->AddChild(Widget);
	}
	
	PlayAnimation(RepliesFadeAnim);
}

void UNarrativeWidgetBase::OnDialoguePlayerLineStarted(UDialogue* Dialogue, UDialogueNode_Player* Node, const FDialogueLine& DialogueLine)
{
	DialogueNameText->SetText(INVTEXT("You"));
	DialogueNameText->SetColorAndOpacity(DialoguePlayerColor);
	DialogueTitleText->SetVisibility(ESlateVisibility::Collapsed);
	DialogueLineText->SetText(InjectTextVariables(DialogueLine.Text));
	PlayAnimation(DialogueFadeAnim);
}

void UNarrativeWidgetBase::OnDialoguePlayerLineFinished(UDialogue* Dialogue, UDialogueNode_Player* Node, const FDialogueLine& DialogueLine)
{
	PlayAnimationReverse(RepliesFadeAnim);
}

void UNarrativeWidgetBase::OnDialogueNPCLineStarted(UDialogue* Dialogue, UDialogueNode_NPC* Node, const FDialogueLine& DialogueLine, const FSpeakerInfo& Speaker)
{
	DialogueNameText->SetText(FText::FromName(Speaker.SpeakerID));
	DialogueNameText->SetColorAndOpacity(DialogueColor);

	DialogueTitleText->SetText(Speaker.SpeakerName);
	DialogueTitleText->SetVisibility(Speaker.SpeakerName.IsEmptyOrWhitespace()
		? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);

	DialogueLineText->SetText(InjectTextVariables(DialogueLine.Text));
	PlayAnimation(DialogueFadeAnim);
}

void UNarrativeWidgetBase::OnLineSkipClicked()
{
	NarrativeComponent->CurrentDialogue->SkipCurrentLine();
}

void UNarrativeWidgetBase::InitWidget()
{
	Super::InitWidget();
	PlayerChar = AToroPlayerCharacter::Get(this);
	NarrativeComponent = UToroNarrativeComponent::Get(this);

	NarrativeComponent->OnQuestNewState.AddDynamic(this, &ThisClass::OnQuestNewState);
	NarrativeComponent->OnQuestTaskCompleted.AddDynamic(this, &ThisClass::OnQuestTaskCompleted);
	NarrativeComponent->OnQuestTaskProgressChanged.AddDynamic(this, &ThisClass::OnQuestTaskProgressChanged);
	NarrativeComponent->OnQuestSucceeded.AddDynamic(this, &ThisClass::OnQuestSucceeded);

	NarrativeComponent->OnDialogueBegan.AddDynamic(this, &ThisClass::OnDialogueBegan);
	NarrativeComponent->OnDialogueFinished.AddDynamic(this, &ThisClass::OnDialogueFinished);
	NarrativeComponent->OnDialogueRepliesAvailable.AddDynamic(this, &ThisClass::OnDialogueRepliesAvailable);
	NarrativeComponent->OnPlayerDialogueLineStarted.AddDynamic(this, &ThisClass::OnDialoguePlayerLineStarted);
	NarrativeComponent->OnPlayerDialogueLineFinished.AddDynamic(this, &ThisClass::OnDialoguePlayerLineFinished);
	NarrativeComponent->OnNPCDialogueLineStarted.AddDynamic(this, &ThisClass::OnDialogueNPCLineStarted);

	SkipLineButton->OnClicked.AddDynamic(this, &ThisClass::OnLineSkipClicked);
}