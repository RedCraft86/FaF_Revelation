// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "InventoryWidget.generated.h"

class UImage;
class UBorder;
class UButton;
class UTextBlock;
class UCommonLabeledButton;
class UExpressiveTextBlock;

UCLASS(Abstract, Blueprintable, BlueprintType)
class UInventoryEntryWidget final : public UUserWidget
{
	GENERATED_BODY()

	friend class UInventoryWidget;

public:

	UInventoryEntryWidget(const FObjectInitializer& ObjectInit);

	DECLARE_DELEGATE_OneParam(FOnEntrySelected, const UInventoryEntryWidget*);
	FOnEntrySelected OnEntrySelected;

private:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<UTextBlock> NameText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<UImage> IconImage;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<UButton> SelectButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<UBorder> SelectBorder;

	UFUNCTION() void OnSelectClicked() const;

	void InitEntry(const FGameplayTag& Key) const;
	void SetSelectionState(const bool bSelected) const;

	virtual void NativeConstruct() override;
};

UCLASS(Abstract, Blueprintable, BlueprintType)
class UInventoryWidget final : public UToroUserWidget
{
	GENERATED_BODY()

public:

	UInventoryWidget(const FObjectInitializer& ObjectInit);

private:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<UPanelWidget> EntryContainer;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<UTextBlock> TitleText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<UExpressiveTextBlock> DescText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true, BindWidget))
		TObjectPtr<UCommonLabeledButton> EquipButton;

	UPROPERTY(EditAnywhere, Category = Settings)
		TSubclassOf<UInventoryEntryWidget> EntryWidgetClass;

	UPROPERTY(Transient)
		TMap<FGameplayTag, TObjectPtr<UInventoryEntryWidget>> Entries;

	UPROPERTY(Transient)
		FGameplayTag Selection;

	UPROPERTY(Transient)
		TWeakObjectPtr<class UInventoryManager> Inventory;

	void DisplayItem(const FGameplayTag& Key) const;
	void OnEntrySelected(const UInventoryEntryWidget* Entry);
	void OnEquipClicked() const;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
};
