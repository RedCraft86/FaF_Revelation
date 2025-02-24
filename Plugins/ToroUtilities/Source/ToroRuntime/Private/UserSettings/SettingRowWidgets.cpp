// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserSettings/SettingRowWidgets.h"

void USettingTooltipBase::UpdateTooltip(const USettingRowBase* InRow) const
{
	if (InRow)
	{
		DescText->SetText(InRow->GetToolTipText());
		PerformanceText->SetVisibility(ESlateVisibility::Collapsed);
		switch (InRow->Performance)
		{
		case ESettingPerformance::Low:
			{
				PerformanceText->SetText(NSLOCTEXT("Toro", "PerformanceLow", "Impact: Low"));
				PerformanceText->SetColorAndOpacity(FLinearColor::Green);
			} break;
		case ESettingPerformance::Medium:
			{
				PerformanceText->SetText(NSLOCTEXT("Toro", "PerformanceMedium", "Impact: Medium"));
				PerformanceText->SetColorAndOpacity(FLinearColor::Yellow);
			} break;
		case ESettingPerformance::High:
			{
				PerformanceText->SetText(NSLOCTEXT("Toro", "PerformanceHigh", "Impact: High"));
				PerformanceText->SetColorAndOpacity(FLinearColor::Red);
			} break;
		default:
			PerformanceText->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
}

void USettingRowBase::OnLabelHover()
{
	if (const USettingTooltipBase* SettingTooltip = Cast<USettingTooltipBase>(GetToolTip()))
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
