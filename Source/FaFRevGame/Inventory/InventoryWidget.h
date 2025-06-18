// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "SlotWidgets.h"
#include "ToroSettings.h"
#include "InventoryItemData.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "InventoryWidget.generated.h"

class UButton;
class UWidgetSwitcher;

UCLASS(Abstract)
class FAFREVGAME_API UInventoryWidget final : public UToroWidgetBase
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
		TObjectPtr<UTextBlock> DescText; // TODO UExprTextBlock

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
		TSubclassOf<UItemSlotWidget> ItemWidgetClass;

	UPROPERTY(EditAnywhere, Category = Settings)
		TSubclassOf<UArchiveSlotWidget> ArchiveWidgetClass;

	void SelectItem(const TSoftObjectPtr<UInvItemData>& InItem);

private:

	UPROPERTY() bool bArchiveTab;
	UPROPERTY() TSoftObjectPtr<UInvItemData> SelectedItem;
	UPROPERTY() TSoftObjectPtr<UInvItemData> SelectedArchive;
	UPROPERTY(Transient) TObjectPtr<UInventoryComponent> Inventory;
	UPROPERTY(Transient) TMap<TSoftObjectPtr<UInvItemData>, TObjectPtr<UItemSlotWidget>> ItemWidgets;
	UPROPERTY(Transient) TMap<TSoftObjectPtr<UInvItemData>, TObjectPtr<UArchiveSlotWidget>> ArchiveWidgets;

	UFUNCTION() void OnItemTab();
	UFUNCTION() void OnArchiveTab();
	UFUNCTION() void OnEquipClicked();

	void UpdateInfo();
	void UpdateSlots();
	virtual void InitWidget() override;
	virtual void InternalProcessActivation() override;
	virtual bool CanCreateWidget() const override { return UToroSettings::Get()->IsOnGameplayMap(this); }
};
