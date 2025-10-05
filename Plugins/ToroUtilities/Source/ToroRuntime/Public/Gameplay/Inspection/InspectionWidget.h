// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "DataTypes/GameInputConfig.h"
#include "Inventory/InventoryAsset.h"
#include "UserInterface/ToroManagedWidget.h"
#include "InspectionWidget.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API UInspectionWidget final : public UToroManagedWidget
{
	GENERATED_BODY()

public:

	UInspectionWidget(const FObjectInitializer& ObjectInitializer);

	void ShowWidget(const UInventoryAsset* InArchive);

	virtual void PopWidget() override;

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UImage> ThumbnailImage;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> TitleText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> DescText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> ContentText;

	FGameInputConfig InputConfig;

	virtual void PushWidget() override;
};
