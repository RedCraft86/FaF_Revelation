// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserWidgets/ToroWidgetBase.h"
#include "InventoryWidgetBase.generated.h"

class UImage;
class UButton;
class USizeBox;
class UTextBlock;
class UExprTextBlock;

UCLASS(Abstract)
class TORORUNTIME_API UInvSlotWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:

	UInvSlotWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
		, NormalColor(0.025f, 0.025f, 0.025f), SelectedColor(0.75f, 0.75f, 0.0f)
	{}

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UImage> EquipImage;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UImage> ThumbnailImage;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UImage> OutlineImage;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> AmountText;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> SlotButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		FLinearColor NormalColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		FLinearColor SelectedColor;
	
	UPROPERTY() FGuid SlotID;
	UPROPERTY(Transient) TObjectPtr<UInventoryWidgetBase> ParentUI;
	
	UFUNCTION() void OnClicked();
	void MarkSelected(const bool bSelected) const;
	void InitData(const FGuid& InSlotID, const FInvSlotData& SlotData, const bool bEquipped);
};

UCLASS(Abstract)
class TORORUNTIME_API UInventoryWidgetBase : public UToroWidget
{
	GENERATED_BODY()

	friend class UInventoryComponent;

public:

	UInventoryWidgetBase(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer), ImageDescHeight(200.0f)
	{
		ZOrder = 57;
	}

	// Left Panel
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<class UComboBoxString> SlotFilter;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UPanelWidget> SlotsBox;
	// ~Left Panel

	// Middle Panel
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UImage> PreviewImage;
	// ~Middle Panel

	// Right Panel
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> NameText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UExprTextBlock> DescText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> TypeText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UWidget> EquippedBox;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UWidget> MultiUseBox;
	// ~Right Panel

	// Right Panel Buttons
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> UseButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> UseLabelText;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> ViewButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> ViewLabelText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> EquipButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> EquipLabelText;
	// ~Right Panel Buttons

	// View Panel
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UImage> ViewImage;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> ViewTitleText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UExprTextBlock> ViewDescText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USizeBox> ViewDescBox;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> FinishViewButton;
	// ~View Panel

	// Animations
	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> SlotFadeAnim;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> DescFadeAnim;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> ViewFadeAnim;
	// ~Animations

	UPROPERTY(EditAnywhere, Category = Settings)
		TSubclassOf<UInvSlotWidgetBase> SlotWidgetClass;

	UPROPERTY(EditAnywhere, Category = Settings)
		float ImageDescHeight;

	void SelectSlot(const FGuid& InKey);

protected:

	FGuid SelectedKey;
	FGuid EquipmentKey;
	TArray<FGuid> SlotKeys;
	UPROPERTY(Transient) TObjectPtr<UInventoryComponent> Inventory;
	UPROPERTY(Transient) TMap<FGuid, TObjectPtr<UInvSlotWidgetBase>> SlotWidgets;

	UFUNCTION() void OnUseClicked();
	UFUNCTION() void OnViewClicked();
	UFUNCTION() void OnEquipClicked();
	UFUNCTION() void OnFinishViewClicked();
	UFUNCTION() void OnTypeSelected(FString SelectedItem, ESelectInfo::Type SelectionType);

	void RefreshInfo();
	void RefreshSlots();
	virtual void InitWidget() override;
	virtual void InternalProcessActivation() override;
	virtual void InternalProcessDeactivation() override;
};
