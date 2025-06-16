// Copyright (C) RedCraft86. All Rights Reserved.

#include "InteractionWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"

UInteractionWidget::UInteractionWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), DefaultIconSize(35.0f)
{
}

void UInteractionWidget::UpdateUI(const bool bEnabled, const FInteractionData& Data)
{
	SetHidden(!bEnabled);
	if (!bEnabled) return;
	if (Data.Target && Data.Info.bEnabled)
	{
		const bool bHasIcon = IsValid(Data.Info.Icon);
		if (InteractBrush.GetResourceObject() != (bHasIcon ? Data.Info.Icon : DefaultIcon))
		{
			InteractBrush.SetResourceObject(bHasIcon ? Data.Info.Icon : DefaultIcon);
			InteractBrush.SetImageSize(bHasIcon ? Data.Info.IconSize : DefaultIconSize);
			InteractIcon->SetBrush(InteractBrush);
		}
		InteractIcon->SetVisibility(ESlateVisibility::Visible);

		InteractLabel->SetText(Data.Info.Label);
		InteractLabel->SetVisibility(Data.Info.Label.IsEmptyOrWhitespace()
			? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
		if (!Data.Info.Label.IsEmptyOrWhitespace())
		{
			if (UCanvasPanelSlot* AsSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(InteractLabel))
			{
				const float Ratio = InteractBrush.GetImageSize().SizeSquared() / DefaultIconSize.SizeSquared();
				AsSlot->SetPosition(Data.Info.LabelOffset + FVector2D(Ratio * 16.0f));
			}
		}

		Crosshair->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		InteractIcon->SetVisibility(ESlateVisibility::Collapsed);
		InteractLabel->SetVisibility(ESlateVisibility::Collapsed);
		Crosshair->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UInteractionWidget::InitWidget()
{
	Super::InitWidget();
	if (UInteractionComponent* Interaction = UInteractionComponent::Get(this))
	{
		Interaction->OnUpdate.BindUObject(this, &ThisClass::UpdateUI);
	}
}

void UInteractionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InteractBrush = InteractIcon->GetBrush();
}
