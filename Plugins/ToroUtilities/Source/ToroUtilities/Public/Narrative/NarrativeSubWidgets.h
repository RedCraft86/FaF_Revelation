// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "NarrativeDataTask.h"
#include "NarrativeComponent.h"
#include "Blueprint/UserWidget.h"
#include "NarrativeSubWidgets.generated.h"

class UButton;
class UTextBlock;
class UNarrativeWidget;
class UDialogueNode_Player;
class UNarrativeTask;
class UQuestBranch;
class UDialogue;

UCLASS(Abstract)
class TOROUTILITIES_API UQuestTaskWidget final : public UUserWidget
{
	GENERATED_BODY()

	friend class UQuestBranchWidget;
	
public:

	UQuestTaskWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> DisplayText;

protected:

	UPROPERTY(Transient) TObjectPtr<UNarrativeWidget> ParentUI;
	UPROPERTY(Transient) TObjectPtr<const UNarrativeTask> TaskObject;

	void InitWidget(UNarrativeWidget* InParent, const UNarrativeTask* InTask);
};

UCLASS(Abstract)
class TOROUTILITIES_API UQuestBranchWidget final : public UUserWidget
{
	GENERATED_BODY()

	friend class UNarrativeWidget;
	
public:

	UQuestBranchWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), bUseTitle(false) {}

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> DisplayText;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UPanelWidget> TaskList;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = QuestBranchWidget)
		TSubclassOf<UQuestTaskWidget> TaskWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = QuestBranchWidget)
		bool bUseTitle;
	
protected:

	UPROPERTY(Transient) TObjectPtr<UNarrativeWidget> ParentUI;
	UPROPERTY(Transient) TObjectPtr<const UQuestBranch> BranchObject;

	void InitWidget(UNarrativeWidget* InParent, const UQuestBranch* InBranch);
};

UCLASS(Abstract)
class TOROUTILITIES_API UDialogueOptionWidget final : public UUserWidget
{
	GENERATED_BODY()

	friend class UNarrativeWidget;
	
public:

	UDialogueOptionWidget(const FObjectInitializer& ObjectInitializer);

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

	UPROPERTY(Transient) TObjectPtr<UNarrativeWidget> ParentUI;
	UPROPERTY(Transient) TObjectPtr<UDialogueNode_Player> ReplyObject;

	UFUNCTION() void OnOptionClicked();
	
	void MarkSelected() const;
	void InitWidget(UNarrativeWidget* InParent, UDialogue* InDialogue, UDialogueNode_Player* InReply);
};