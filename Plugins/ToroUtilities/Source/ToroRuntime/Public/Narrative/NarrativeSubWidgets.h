// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "NarrativeDataTask.h"
#include "Blueprint/UserWidget.h"
#include "NarrativeSubWidgets.generated.h"

class UButton;
class UTextBlock;
class UNarrativeWidgetBase;
class UDialogueNode_Player;
class UNarrativeTask;
class UQuestBranch;
class UDialogue;

UCLASS(Abstract)
class TORORUNTIME_API UQuestTaskWidgetBase final : public UUserWidget
{
	GENERATED_BODY()

	friend class UQuestBranchWidgetBase;
	
public:

	UQuestTaskWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> DisplayText;

protected:

	UPROPERTY(Transient) TObjectPtr<UNarrativeWidgetBase> ParentUI;
	UPROPERTY(Transient) TObjectPtr<const UNarrativeTask> TaskObject;

	void InitWidget(UNarrativeWidgetBase* InParent, const UNarrativeTask* InTask);
};

UCLASS(Abstract)
class TORORUNTIME_API UQuestBranchWidgetBase final : public UUserWidget
{
	GENERATED_BODY()

	friend class UNarrativeWidgetBase;
	
public:

	UQuestBranchWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), bUseTitle(false) {}

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> DisplayText;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UPanelWidget> TaskList;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = QuestBranchWidget)
		TSubclassOf<UQuestTaskWidgetBase> TaskWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = QuestBranchWidget)
		bool bUseTitle;
	
protected:

	UPROPERTY(Transient) TObjectPtr<UNarrativeWidgetBase> ParentUI;
	UPROPERTY(Transient) TObjectPtr<const UQuestBranch> BranchObject;

	void InitWidget(UNarrativeWidgetBase* InParent, const UQuestBranch* InBranch);
};

UCLASS(Abstract)
class TORORUNTIME_API UDialogueOptionWidgetBase final : public UUserWidget
{
	GENERATED_BODY()

	friend class UNarrativeWidgetBase;
	
public:

	UDialogueOptionWidgetBase(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> DisplayText;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> SelectButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = DialogueOptionWidget, meta = (UIMin = 10, ClampMin = 10))
		uint8 MaxCharCount;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = DialogueOptionWidget)
		FLinearColor SelectedColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = DialogueOptionWidget)
		TObjectPtr<const UNarrativeDataTask> ReplyTaskType;

protected:

	UPROPERTY(Transient) TObjectPtr<UNarrativeWidgetBase> ParentUI;
	UPROPERTY(Transient) TObjectPtr<UDialogueNode_Player> ReplyObject;

	UFUNCTION() void OnOptionClicked();
	
	void MarkSelected() const;
	void InitWidget(UNarrativeWidgetBase* InParent, UDialogue* InDialogue, UDialogueNode_Player* InReply);
};