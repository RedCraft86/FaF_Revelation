// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroUtilities.h"
#include "InventorySlotWidget.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "InventoryWidget.generated.h"

class UButton;
class UExprTextBlock;
class UWidgetSwitcher;

UCLASS(Abstract)
class TOROSYSTEMS_API UInventoryWidget final : public UToroWidgetBase
{
	GENERATED_BODY()

public:

	UInventoryWidget(const FObjectInitializer& ObjectInitializer);

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
		TObjectPtr<UTextBlock> LabelText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UImage> ThumbnailImage;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> SecretText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UExprTextBlock> DescText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UExprTextBlock> ArDescText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> EquipBtn;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> EquipLabelText;
	// ~Right Panel

	// Animations
	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> SlotAnim;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> InfoAnim;
	// ~Animations

	UPROPERTY(EditAnywhere, Category = Settings)
		TSubclassOf<UInvItemSlotWidget> ItemWidgetClass;

	UPROPERTY(EditAnywhere, Category = Settings)
		TSubclassOf<UInvArchiveSlotWidget> ArchiveWidgetClass;

	void SelectItem(const TSoftObjectPtr<UInventoryAsset>& InItem);

private:

	UPROPERTY() bool bArchiveTab;
	UPROPERTY() TSoftObjectPtr<UInventoryAsset> SelectedItem;
	UPROPERTY() TSoftObjectPtr<UInventoryAsset> SelectedArchive;
	UPROPERTY(Transient) TObjectPtr<class UInventoryComponent> Inventory;
	UPROPERTY(Transient) TMap<TSoftObjectPtr<UInventoryAsset>, TObjectPtr<UInvItemSlotWidget>> ItemWidgets;
	UPROPERTY(Transient) TMap<TSoftObjectPtr<UInventoryAsset>, TObjectPtr<UInvArchiveSlotWidget>> ArchiveWidgets;

	UFUNCTION() void OnItemTab();
	UFUNCTION() void OnArchiveTab();
	UFUNCTION() void OnEquipClicked();

	void UpdateInfo();
	void UpdateSlots();
	virtual void InitWidget() override;
	virtual void InternalProcessActivation() override;
	virtual bool CanCreateWidget() const override { return UToroUtilSettings::Get()->IsOnGameplayMap(this); }
};
