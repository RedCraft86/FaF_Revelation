// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Blueprint/UserWidget.h"
#include "ToroButtonList.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API UToroButtonEntry final : public UUserWidget
{
	GENERATED_BODY()

	friend class UToroButtonList;

public:

	UToroButtonEntry(const FObjectInitializer& ObjectInitializer)
		: UUserWidget(ObjectInitializer), bSelected(false)
	{}

	DECLARE_DELEGATE_OneParam(FOnClicked, UToroButtonEntry*);
	FOnClicked OnClicked;

	void SelectButton(bool bImmediate = false);
	void DeselectButton(bool bImmediate = false);
	void SetDisplayData(const FText& InText, const FSlateFontInfo& InFont) const;

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> Button;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> Label;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> SelectAnim;

	bool bSelected;

	UFUNCTION() void OnButtonClicked();
	virtual void NativeConstruct() override;
};

UCLASS(Abstract, HideCategories = (Interaction, Performance, Localization), meta = (PrioritizeCategories = "ButtonList"))
class TORORUNTIME_API UToroButtonList final : public UUserWidget
{
	GENERATED_BODY()

public:

	UToroButtonList(const FObjectInitializer& ObjectInitializer);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectionBP, const int32, Index);
	UPROPERTY(BlueprintAssignable, DisplayName = "On Selection") FOnSelectionBP OnSelectionBP;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnSelection, const int32);
	FOnSelection OnSelection;

	UFUNCTION(BlueprintPure, Category = ButtonList)
		int32 GetSelectedIndex() const { return SelectedIndex; }

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UVerticalBox> Container;

	UPROPERTY(EditAnywhere, Category = ButtonList)
		TSubclassOf<UToroButtonEntry> EntryClass;

	UPROPERTY(EditAnywhere, Category = ButtonList)
		FSlateChildSize EntrySize;

	UPROPERTY(EditAnywhere, Category = ButtonList)
		FMargin EntryPadding;

	UPROPERTY(EditAnywhere, Category = ButtonList)
		FSlateFontInfo EntryFont;

	UPROPERTY(EditAnywhere, Category = ButtonList)
		TArray<FText> Entries;

	UPROPERTY(BlueprintReadOnly, Category = ButtonList)
		TArray<TObjectPtr<UToroButtonEntry>> Buttons;

	int32 SelectedIndex;

	void OnSelectionMade(UToroButtonEntry* Button);
	virtual void NativePreConstruct() override;
};
