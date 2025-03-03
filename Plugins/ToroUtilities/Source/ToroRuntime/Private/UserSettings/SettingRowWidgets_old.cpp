// Copyright (C) RedCraft86. All Rights Reserved.

// ReSharper disable CppMemberFunctionMayBeStatic
// ReSharper disable CppMemberFunctionMayBeConst
#include "UserSettings/SettingRowWidgets_old.h"
#include "UserSettings/ToroUserSettings.h"
#include "Animation/WidgetAnimation.h"
#include "Components/SpinBox.h"
#include "EnhancedCodeFlow.h"

void USettingTooltipBase_old::UpdateTooltip(const USettingRowBase_old* InRow) const
{
	if (InRow)
	{
		DescText->SetText(InRow->GetToolTipText());
		PerformanceText->SetVisibility(ESlateVisibility::HitTestInvisible);
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
			PerformanceText->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

UWidget* USettingRowBase_old::CreateTooltip()
{
	USettingTooltipBase_old* Widget = CreateWidget<USettingTooltipBase_old>(this, TooltipWidgetClass);
	Widget->UpdateTooltip(this);
	return Widget;
}

void USettingRowBase_old::UpdateWidget()
{
	bSetByCode = true;
	ResetButton->SetIsEnabled(!IsDefaultValue());
	ResetButton->SetVisibility(ResetButton->GetIsEnabled() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	bSetByCode = false;
}

void USettingRowBase_old::NativeConstruct()
{
	Super::NativeConstruct();
	LabelText->SetVisibility(ESlateVisibility::Visible);
	ResetButton->OnClicked.AddUniqueDynamic(this, &USettingRowBase_old::ResetToDefault);
	FFlow::DelayTicks(this, 1, [this]()
	{
		UpdateWidget();
	});
}

void USettingRowBase_old::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (LabelText) LabelText->SetText(Label);
}

void USettingRowBase_old::SynchronizeProperties()
{
	ToolTipWidgetDelegate.BindDynamic(this, &USettingRowBase_old::CreateTooltip);
	Super::SynchronizeProperties();
}

void UToggleSettingBase_old::UpdateWidget()
{
	bSetByCode = true;
	bCurrentValue = GetterFunction();
	Super::UpdateWidget();
}

void UToggleSettingBase_old::ResetToDefault()
{
	OnToggleClicked();
	Super::UpdateWidget();
}

void UToggleSettingBase_old::OnToggleClicked()
{
	if (bSetByCode) return;
	bCurrentValue = !bCurrentValue;
	PlayAnimation(ToggleAnim, 0.0f, 1, bCurrentValue
		? EUMGSequencePlayMode::Forward : EUMGSequencePlayMode::Reverse);
	SetterFunction(bCurrentValue);
}

void UToggleSettingBase_old::NativeConstruct()
{
	Super::NativeConstruct();
	ToggleButton->OnClicked.AddUniqueDynamic(this, &UToggleSettingBase_old::OnToggleClicked);
}

void UToggleSettingBase_old::NativePreConstruct()
{
	Super::NativePreConstruct();
	bCurrentValue = bDefaultValue;
	SetAnimationCurrentTime(ToggleAnim, bCurrentValue
		? ToggleAnim->GetEndTime() : ToggleAnim->GetStartTime());
}

void USliderSettingBase_old::UpdateWidget()
{
	bSetByCode = true;
	SliderSpinBox->SetValue(GetterFunction());
	Super::UpdateWidget();
}

bool USliderSettingBase_old::IsDefaultValue()
{
	return FMath::IsNearlyEqual(SliderSpinBox->GetValue(), DefaultValue);
}

void USliderSettingBase_old::ResetToDefault()
{
	SliderSpinBox->SetValue(DefaultValue);
	Super::UpdateWidget();
	ApplyChange();
}

void USliderSettingBase_old::ApplyChange() const
{
	if (bSetByCode) return;
	SetterFunction(SliderSpinBox->GetValue());
}

void USliderSettingBase_old::OnSliderValueChanged(float Value)
{
	//ApplyChange();
}

void USliderSettingBase_old::OnSliderMovementEnd(float Value)
{
	ApplyChange();
}

void USliderSettingBase_old::OnSliderValueCommitted(float Value, ETextCommit::Type Type)
{
	ApplyChange();
}

void USliderSettingBase_old::NativeConstruct()
{
	Super::NativeConstruct();
	SliderSpinBox->OnValueChanged.AddUniqueDynamic(this, &USliderSettingBase_old::OnSliderValueChanged);
	SliderSpinBox->OnEndSliderMovement.AddUniqueDynamic(this, &USliderSettingBase_old::OnSliderMovementEnd);
	SliderSpinBox->OnValueCommitted.AddUniqueDynamic(this, &USliderSettingBase_old::OnSliderValueCommitted);
}

void USliderSettingBase_old::NativePreConstruct()
{
	Super::NativePreConstruct();
	SliderSpinBox->SetMinValue(Range.GetMin());
	SliderSpinBox->SetMaxValue(Range.GetMax());
	SliderSpinBox->SetMinSliderValue(Range.GetMin());
	SliderSpinBox->SetMaxSliderValue(Range.GetMax());
	SliderSpinBox->SetMaxFractionalDigits(NumOfDecimals);
	SliderSpinBox->SetMinFractionalDigits(NumOfDecimals > 0 ? 1 : 0);
	SliderSpinBox->SetDelta(NumOfDecimals > 0 ? 0.0f : 1.0f);
	SliderSpinBox->SetValue(FMath::Clamp(DefaultValue, Range.GetMin(), Range.GetMax()));
}

void USwitcherSettingBase_old::UpdateWidget()
{
	bSetByCode = true;
	Value = GetterFunction();
	ValueText->SetText(Options[Value]);
	Super::UpdateWidget();
}

void USwitcherSettingBase_old::ResetToDefault()
{
	Value = DefaultOption;
	Super::UpdateWidget();
	ApplyChange();
}

void USwitcherSettingBase_old::ApplyChange() const
{
	SetterFunction(Value);
	ValueText->SetText(Options[Value]);
}

void USwitcherSettingBase_old::OnPrevClicked()
{
	if (bSetByCode) return;

	Value--;
	if (Value < 0) Value = Options.Num() - 1;
	ApplyChange();
}

void USwitcherSettingBase_old::OnNextClicked()
{
	if (bSetByCode) return;

	Value++;
	if (Value >= Options.Num()) Value = 0;
	ApplyChange();
}

void USwitcherSettingBase_old::NativeConstruct()
{
	Super::NativeConstruct();
	PrevButton->OnClicked.AddUniqueDynamic(this, &USwitcherSettingBase_old::OnPrevClicked);
	NextButton->OnClicked.AddUniqueDynamic(this, &USwitcherSettingBase_old::OnNextClicked);
}

void USwitcherSettingBase_old::NativePreConstruct()
{
	Super::NativePreConstruct();
	Value = FMath::Clamp(DefaultOption, 0, Options.Num() - 1);
	ValueText->SetText(Options[Value]);
}

void UComboSettingBase_old::UpdateWidget()
{
	bSetByCode = true;
	SelectionBox->SetSelectedOption(GetterFunction());
	Super::UpdateWidget();
}

bool UComboSettingBase_old::IsDefaultValue()
{
	return SelectionBox->GetSelectedOption() == DefaultOption;
}

void UComboSettingBase_old::ResetToDefault()
{
	SelectionBox->SetSelectedOption(DefaultOption);
	OnSelectionChanged(DefaultOption, ESelectInfo::Type::Direct);
	Super::UpdateWidget();
}

void UComboSettingBase_old::OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (bSetByCode) return;
	SetterFunction(SelectedItem);
	UpdateWidget();
}

void UComboSettingBase_old::NativeConstruct()
{
	Super::NativeConstruct();
	SelectionBox->OnSelectionChanged.AddUniqueDynamic(this, &UComboSettingBase_old::OnSelectionChanged);
}

void UComboSettingBase_old::NativePreConstruct()
{
	Super::NativePreConstruct();
	SelectionBox->ClearOptions();
	for (const FString& Option : Options)
	{
		SelectionBox->AddOption(Option);
	}
	if (!Options.Contains(DefaultOption))
	{
		DefaultOption = Options.IsEmpty() ? TEXT("") : Options[0];
		SelectionBox->SetSelectedOption(DefaultOption);
	}
}

void UResolutionSettingBase_old::UpdateWidget()
{
	bSetByCode = true;
	if (const UToroUserSettings* Settings = UToroUserSettings::Get())
	{
		const FIntPoint Res = Settings->GetScreenResolution();
		SelectionBox->SetSelectedOption(FString::Printf(TEXT("%dx%d"), Res.X, Res.Y));
		Value = SelectionBox->GetSelectedOption();
	}
	bSetByCode = false;
}

void UResolutionSettingBase_old::OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (bSetByCode) return;
	if (UToroUserSettings* Settings = UToroUserSettings::Get())
	{
		FString StrX, StrY;
		SelectedItem.Split(TEXT("x"), &StrX, &StrY);
		const FIntPoint Res{FCString::Atoi(*StrX), FCString::Atoi(*StrY)};

		Settings->SetScreenResolution(Res);
		Settings->SetFullscreenMode(Res == Settings->FullscreenRes
#if WITH_EDITOR
			? EWindowMode::Type::WindowedFullscreen : EWindowMode::Type::Windowed);
#else
			? EWindowMode::Type::Fullscreen : EWindowMode::Type::Windowed);
#endif
		Settings->ApplyResolutionSettings(false);
		OnResolutionChanged.Broadcast(Value);
		Value = SelectedItem;
	}
}

void UResolutionSettingBase_old::NativePreConstruct()
{
	USettingRowBase_old::NativePreConstruct();
	UToroUserSettings::CheckSupportedResolutions();

	SelectionBox->ClearOptions();
	for (uint8 i = 0; i < UToroUserSettings::SupportedResolutions.Num(); i++)
	{
		const FIntPoint& Res = UToroUserSettings::SupportedResolutions[i];
		SelectionBox->AddOption(FString::Printf(TEXT("%dx%d"), Res.X, Res.Y));
	}
	if (!Options.Contains(DefaultOption))
	{
		const FIntPoint& Res = UToroUserSettings::FullscreenRes;
		DefaultOption = FString::Printf(TEXT("%dx%d"), Res.X, Res.Y);
		SelectionBox->SetSelectedOption(DefaultOption);
	}
	Value = SelectionBox->GetSelectedOption();
}

void UImageFidelitySettingBase_old::UpdateWidget()
{
	bSetByCode = true;
	if (const UToroUserSettings* Settings = UToroUserSettings::Get())
	{
		SelectionBox->SetSelectedOption(ModeToName.FindRef(Settings->GetImageFidelityMode()));
	}
	bSetByCode = false;
}

void UImageFidelitySettingBase_old::OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (bSetByCode) return;
	if (UToroUserSettings* Settings = UToroUserSettings::Get())
	{
		Settings->SetImageFidelityMode(*ModeToName.FindKey(SelectedItem));
	}
}

void UImageFidelitySettingBase_old::NativePreConstruct()
{
	USettingRowBase_old::NativePreConstruct();
	UToroUserSettings::CheckSupportedFidelityModes();

	SelectionBox->ClearOptions();
	for (uint8 i = 0; i < UToroUserSettings::SupportedFidelityModes.Num(); i++)
	{
		SelectionBox->AddOption(ModeToName.FindRef(UToroUserSettings::SupportedFidelityModes[i]));
	}
	if (!Options.Contains(DefaultOption))
	{
		DefaultOption = ModeToName.FindRef(EImageFidelityMode::TAA);
		SelectionBox->SetSelectedOption(DefaultOption);
	}
}
