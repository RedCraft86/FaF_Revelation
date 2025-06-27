// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroUtilities.h"
#include "InteractionCache.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "InteractionWidget.generated.h"

UCLASS()
class TOROSYSTEMS_API UInteractionWidget final : public UToroWidgetBase
{
	GENERATED_BODY()

	friend class UInteractionComponent;

public:

	UInteractionWidget(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
	{
		ZOrder = 95;
	}

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UImage> Crosshair;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UImage> InteractIcon;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> InteractLabel;

	UPROPERTY(EditAnywhere, Category = Settings)
		TObjectPtr<UTexture2D> DefaultIcon;

private:

	void UpdateUI(const bool bEnabled, const FInteractionCache& Data)
	{
		SetHidden(!bEnabled);
		if (bEnabled)
		{
			if (Data.Target && Data.Info.bEnabled)
			{
				InteractLabel->SetText(Data.Info.Label);
				InteractIcon->SetBrushFromTexture(Data.Info.Icon ? Data.Info.Icon : DefaultIcon);
				InteractIcon->SetVisibility(ESlateVisibility::Visible);
				Crosshair->SetVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				InteractIcon->SetVisibility(ESlateVisibility::Collapsed);
				InteractLabel->SetVisibility(ESlateVisibility::Collapsed);
				Crosshair->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
		}
	}

	virtual bool CanCreateWidget() const override { return UToroUtilSettings::Get()->IsOnGameplayMap(this); }
};
