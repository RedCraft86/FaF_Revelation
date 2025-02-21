// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserSettings/SettingRowWidgets.h"

void USettingTooltipBase::UpdateTooltip(const USettingRowBase* InRow) const
{
	if (InRow)
	{
		TooltipText->SetText(InRow->ToolTipText);
		PerformanceText->SetVisibility(ESlateVisibility::Collapsed);
		switch (InRow->Performance)
		{
		case ESettingPerformance::Low:
			{
				PerformanceText->SetText(INVTEXT("Impact: Low"));
				PerformanceText->SetColorAndOpacity(FLinearColor::Green);
			} break;
		case ESettingPerformance::Medium:
			{
				PerformanceText->SetText(INVTEXT("Impact: Medium"));
				PerformanceText->SetColorAndOpacity(FLinearColor::Yellow);
			} break;
		case ESettingPerformance::High:
			{
				PerformanceText->SetText(INVTEXT("Impact: High"));
				PerformanceText->SetColorAndOpacity(FLinearColor::Red);
			} break;
		case default:
			PerformanceText->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
}

void USettingRowBase::OnLabelHover()
{
	if (const USettingTooltipBase* SettingTooltip = Cast<USettingTooltipBase>(ToolTipWidget))
	{
		SettingTooltip->UpdateTooltip(this);
	}
}

void USettingRowBase::NativeConstruct()
{
	Super::NativeConstruct();
	ResetButton->OnClicked.AddUniqueDynamic(this, &USettingRowBase::OnReset);
	LabelButton->OnHovered.AddUniqueDynamic(this, &USettingRowBase::OnLabelHover);
}

void USettingRowBase::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (LabelText) LabelText->SetText(Label);
}
