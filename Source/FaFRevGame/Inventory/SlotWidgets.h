// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InventoryItemData.h"
#include "Blueprint/UserWidget.h"
#include "SlotWidgets.generated.h"

class UImage;
class UTextBlock;
class UInventoryWidget;

UCLASS(NotBlueprintable)
class FAFREVGAME_API USlotWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent)
		void MarkSelected(const bool bSelected);

protected:

	UPROPERTY() TSoftObjectPtr<UInvItemData> ItemData;
	UPROPERTY(Transient) TObjectPtr<UInventoryWidget> ParentWidget;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& MouseEvent) override;
};

UCLASS(Abstract)
class FAFREVGAME_API UItemSlotWidget final : public USlotWidgetBase
{
	GENERATED_BODY()

public:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UImage> ThumbnailImage;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UImage> EquipImage;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> AmountText;

	void MarkEquipped(const bool bEquipped);
	void InitWidget(UInventoryWidget* Owner, const TSoftObjectPtr<UInvItemData>& InItem, const uint8 Amount, const bool bEquipped);
};

UCLASS(Abstract)
class FAFREVGAME_API UArchiveSlotWidget final : public USlotWidgetBase
{
	GENERATED_BODY()

public:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> ItemText;

	void InitWidget(UInventoryWidget* Owner, const TSoftObjectPtr<UInvItemData>& InItem);
};