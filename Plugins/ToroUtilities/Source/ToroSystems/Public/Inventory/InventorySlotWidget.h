// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InventoryAsset.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

class UImage;
class UTextBlock;
class UInventoryWidget;

UCLASS(NotBlueprintable)
class TOROSYSTEMS_API UInvSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent)
		void MarkSelected(const bool bSelected);

protected:

	UPROPERTY() TSoftObjectPtr<UInventoryAsset> InvAsset;
	UPROPERTY(Transient) TObjectPtr<UInventoryWidget> ParentWidget;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& MouseEvent) override;
};

UCLASS(Abstract)
class TOROSYSTEMS_API UInvItemSlotWidget final : public UInvSlotWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UImage> ThumbnailImage;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UImage> EquipImage;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> AmountText;

	void MarkEquipped(const bool bEquipped) const;
	void InitWidget(UInventoryWidget* Owner, const TSoftObjectPtr<UInventoryAsset>& InItem, const uint8 Amount, const bool bEquipped);
};

UCLASS(Abstract)
class TOROSYSTEMS_API UInvArchiveSlotWidget final : public UInvSlotWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> ItemText;

	void InitWidget(UInventoryWidget* Owner, const TSoftObjectPtr<UInventoryAsset>& InItem);
};