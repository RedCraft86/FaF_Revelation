// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroUtilities.h"
#include "Components/TextBlock.h"
#include "DataTypes/InputModeData.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "InspectionWidget.generated.h"

UCLASS(Abstract)
class FAFREVGAME_API UInspectionWidget final : public UToroWidgetBase
{
	GENERATED_BODY()

	friend class AInspectableActor;

public:

	UInspectionWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> TitleText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> ContentText; // TODO UExprTextBlock

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> SecretText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> SecretAnim;

	UPROPERTY(EditAnywhere, Category = Settings)
		TObjectPtr<USoundBase> SecretSound;

private:

	UPROPERTY() bool bSecretFound;
	UPROPERTY() FGameInputModeData CachedInputMode;

	void MarkSecretFound();
	void LoadData(const class UInvItemData* InData, const bool bKnowsSecret);

	virtual void InternalProcessActivation() override;
	virtual void InternalProcessDeactivation() override;
	virtual bool CanCreateWidget() const override { return UToroUtilSettings::Get()->IsOnGameplayMap(this); }
};
