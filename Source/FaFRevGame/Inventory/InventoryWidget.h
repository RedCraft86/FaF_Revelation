// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroSettings.h"
#include "Components/Image.h"
#include "InventoryItemData.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "UserWidgets/ExprTextBlock.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "InventoryWidget.generated.h"

UCLASS(Abstract)
class FAFREVGAME_API UInvSlotWidget final : public UUserWidget
{
	GENERATED_BODY()

public:

	UInvSlotWidget(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer) {}
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UImage> ThumbnailImage;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> ItemText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> AmountText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UImage> EquipImage;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> SlotButton;
	
	UPROPERTY() TSoftObjectPtr<UInventoryItemData> SlotItem;
	UPROPERTY(Transient) TObjectPtr<class UInventoryWidget> ParentUI;

	UFUNCTION(BlueprintImplementableEvent)
		void MarkSelected(const bool bSelected) const;
	
	UFUNCTION() void OnClicked();
	void InitData(const TSoftObjectPtr<UInventoryItemData>& Item, const uint8 Amount, const bool bEquipped);
	void SetEquipped(bool bEquipped);
};

UCLASS(Abstract)
class FAFREVGAME_API UInventoryWidget final : public UToroWidgetBase
{
	GENERATED_BODY()

	friend class UGameInventory;

public:

	UInventoryWidget(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer), bArchiveTab(false)
	{
		ZOrder = 47;
		bAutoActivate = false;
	}

	// Left Panel
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> ItemBtn;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> ArchiveBtn;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UWidgetSwitcher> TabSwitch;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UPanelWidget> ItemBox;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UPanelWidget> ArchiveBox;
	// ~Left Panel

	// Right Panel
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> NameText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UExprTextBlock> DescText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UImage> ViewImage;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> EquipButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> EquipLabelText;
	// ~Right Panel
	
	// Animations
	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> SlotFadeAnim;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> DescFadeAnim;
	// ~Animations

	UPROPERTY(EditAnywhere, Category = Settings)
		TSubclassOf<UInvSlotWidget> ItemWidgetClass;

	UPROPERTY(EditAnywhere, Category = Settings)
		TSubclassOf<UInvSlotWidget> ArchiveWidgetClass;

	void SelectSlot(const TSoftObjectPtr<UInventoryItemData>& InKey);
	void GoToArchive(const TSoftObjectPtr<UInventoryItemData>& InKey);

private:

	UPROPERTY() bool bArchiveTab;
	UPROPERTY() TSoftObjectPtr<UInventoryItemData> SelectedItem;
	UPROPERTY() TSoftObjectPtr<UInventoryItemData> SelectedArchive;
	UPROPERTY(Transient) TMap<TSoftObjectPtr<UInventoryItemData>, TObjectPtr<UInvSlotWidget>> ItemWidgets;
	UPROPERTY(Transient) TMap<TSoftObjectPtr<UInventoryItemData>, TObjectPtr<UInvSlotWidget>> ArchiveWidgets;
	UPROPERTY(Transient) TObjectPtr<UGameInventory> Inventory;
	
	UFUNCTION() void OnItemTab();
	UFUNCTION() void OnArchiveTab();
	UFUNCTION() void OnEquipClicked();

	void RefreshSlots();
	void RefreshInfo();
	virtual void InitWidget() override;
	virtual void InternalProcessActivation() override;
	virtual bool CanCreateWidget() const override { return UToroSettings::Get()->IsOnGameplayMap(this); }
};
