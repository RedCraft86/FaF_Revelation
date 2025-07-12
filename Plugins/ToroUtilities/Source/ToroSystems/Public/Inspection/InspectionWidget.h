// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroUtilities.h"
#include "Components/TextBlock.h"
#include "DataTypes/InputModeData.h"
#include "Inventory/InventoryAsset.h"
#include "UserWidgets/ExprTextBlock.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "InspectionWidget.generated.h"

UCLASS(Abstract)
class TOROSYSTEMS_API UInspectionWidget final : public UToroWidgetBase
{
	GENERATED_BODY()

	friend class UInspectionComponent;

public:

	UInspectionWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> TitleText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UExprTextBlock> DescText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UExprTextBlock> ContentText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> SecretText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> SecretAnim;

	UPROPERTY(EditAnywhere, Category = Settings)
		TObjectPtr<USoundBase> SecretSound;

private:

	UPROPERTY() bool bSecretFound;
	UPROPERTY() FGameInputModeData CachedInput;

	void MarkSecretFound();
	void LoadData(const UInventoryAsset* InArchive, const bool bSecretKnown);

	virtual void InternalProcessActivation() override;
	virtual void InternalProcessDeactivation() override;
	virtual bool CanCreateWidget() const override { return UToroUtilSettings::Get()->IsPlayableMap(this); }
};
