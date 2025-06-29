// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "NarrativeDataTask.h"
#include "Blueprint/UserWidget.h"
#include "NarrativeSubWidgets.generated.h"

class UButton;
class UTextBlock;
class UQuestWidget;
class UDialogueWidget;
class UDialogueNode_Player;
class UNarrativeTask;
class UQuestBranch;
class UDialogue;

UCLASS(Abstract)
class TOROSYSTEMS_API UQuestTaskWidget final : public UUserWidget
{
	GENERATED_BODY()

	friend class UQuestBranchWidget;
	
public:

	UQuestTaskWidget(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer) {}

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> DisplayText;

protected:

	UPROPERTY(Transient) TObjectPtr<UQuestWidget> ParentUI;
	UPROPERTY(Transient) TObjectPtr<const UNarrativeTask> TaskObject;

	void InitWidget(UQuestWidget* InParent, const UNarrativeTask* InTask);
};

UCLASS(Abstract)
class TOROSYSTEMS_API UQuestBranchWidget final : public UUserWidget
{
	GENERATED_BODY()

	friend class UQuestWidget;
	
public:

	UQuestBranchWidget(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer), bUseTitle(false) {}

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> DisplayText;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UPanelWidget> TaskList;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		TSubclassOf<UQuestTaskWidget> TaskWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		bool bUseTitle;
	
protected:

	UPROPERTY(Transient) TObjectPtr<UQuestWidget> ParentUI;
	UPROPERTY(Transient) TObjectPtr<const UQuestBranch> BranchObject;

	void InitWidget(UQuestWidget* InParent, const UQuestBranch* InBranch);
};

UCLASS(Abstract)
class TOROSYSTEMS_API UDialogueOptionWidget final : public UUserWidget
{
	GENERATED_BODY()

	friend class UDialogueWidget;
	
public:

	UDialogueOptionWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> DisplayText;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> SelectButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (UIMin = 10, ClampMin = 10))
		uint8 MaxCharCount;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		FLinearColor SelectedColor;

protected:

	UPROPERTY(Transient) TObjectPtr<UDialogueWidget> ParentUI;
	UPROPERTY(Transient) TObjectPtr<UDialogueNode_Player> ReplyObject;

	UFUNCTION() void OnOptionClicked();
	
	void MarkSelected() const;
	void InitWidget(UDialogueWidget* InParent, UDialogue* InDialogue, UDialogueNode_Player* InReply);
};