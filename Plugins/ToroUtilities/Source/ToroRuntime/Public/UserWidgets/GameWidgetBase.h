﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserWidgets/ToroWidgetBase.h"
#include "GameWidgetBase.generated.h"

class UImage;

UCLASS(Abstract)
class TORORUNTIME_API UGameWidgetBase : public UToroWidget
{
	GENERATED_BODY()

public:

	UGameWidgetBase(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UImage> Crosshair;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UImage> InteractIcon;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UWidget> InteractProgress;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<class UExprTextBlock> InteractLabel;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UPanelWidget> InteractWrapper;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<class UProgressBar> StaminaBar;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> InteractAnim;

	UPROPERTY(EditAnywhere, Category = "Settings|Interaction")
		TObjectPtr<UTexture2D> DefaultIcon;

	UPROPERTY(EditAnywhere, Category = "Settings|Interaction")
		FVector2D DefaultIconSize;
	
	UFUNCTION(BlueprintImplementableEvent, meta = (ForceAsFunction))
	void UpdateInteractProgress(const float Progress);
	
	UFUNCTION(BlueprintImplementableEvent, meta = (ForceAsFunction))
	void UpdateInteractLabel(UExprTextBlock* Label, const FVector2D& Offset, const float SizeRatio);

	void SetInteractionHidden(const bool bInHidden);
	void UpdateInteraction(const struct FInteractionData& InData);
	
protected:

	UPROPERTY() bool bInteractionHidden;
	UPROPERTY() FSlateBrush InteractBrush;
	UPROPERTY(Transient) TObjectPtr<class AToroPlayerBase> PlayerChar;
	UPROPERTY(Transient) TObjectPtr<const AWorldSettings> WorldSettings;

	virtual void InitWidget() override;
	virtual bool ShouldBeHidden() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
