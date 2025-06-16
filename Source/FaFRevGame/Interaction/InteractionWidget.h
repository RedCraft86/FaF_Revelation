// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroSettings.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "InteractionComponent.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "InteractionWidget.generated.h"

UCLASS(Abstract)
class FAFREVGAME_API UInteractionWidget final : public UToroWidgetBase
{
	GENERATED_BODY()

public:

	UInteractionWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UImage> Crosshair;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UImage> InteractIcon;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> InteractLabel;

	UPROPERTY(EditAnywhere, Category = Settings)
		TObjectPtr<UTexture2D> DefaultIcon;

	UPROPERTY(EditAnywhere, Category = Settings)
		FVector2D DefaultIconSize;

private:

	UPROPERTY() FSlateBrush InteractBrush;
	void UpdateUI(const bool bEnabled, const FInteractionData& Data);

	virtual void InitWidget() override;
	virtual bool CanCreateWidget() const override { return UToroSettings::Get()->IsOnGameplayMap(this); }
};
