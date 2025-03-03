// Copyright (C) RedCraft86. All Rights Reserved.

// ReSharper disable CppMemberFunctionMayBeStatic
// ReSharper disable CppMemberFunctionMayBeConst
#include "UserSettings/SettingRowWidgets.h"
#include "UserSettings/ToroUserSettings.h"
#include "Animation/WidgetAnimation.h"
#include "Components/SpinBox.h"
#include "EnhancedCodeFlow.h"

void USettingTooltipBase::UpdateTooltip(const USettingRowBase* InRow) const
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

UWidget* USettingRowBase::CreateTooltip()
{
	USettingTooltipBase* Widget = CreateWidget<USettingTooltipBase>(this, TooltipWidgetClass);
	Widget->UpdateTooltip(this);
	return Widget;
}

void USettingRowBase::UpdateWidget()
{
	bSetByCode = true;
	ResetButton->SetIsEnabled(!IsDefaultValue());
	ResetButton->SetVisibility(ResetButton->GetIsEnabled() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	bSetByCode = false;
}

void USettingRowBase::NativeConstruct()
{
	Super::NativeConstruct();
	LabelText->SetVisibility(ESlateVisibility::Visible);
	ResetButton->OnClicked.AddUniqueDynamic(this, &USettingRowBase::ResetToDefault);
	FFlow::DelayTicks(this, 1, [this]()
	{
		UpdateWidget();
	});
}

void USettingRowBase::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (LabelText) LabelText->SetText(Label);
}

void USettingRowBase::SynchronizeProperties()
{
	ToolTipWidgetDelegate.BindDynamic(this, &USettingRowBase::CreateTooltip);
	Super::SynchronizeProperties();
}

void UToggleSettingBase::UpdateWidget()
{
	bSetByCode = true;
	bCurrentValue = GetterFunction();
	Super::UpdateWidget();
}

void UToggleSettingBase::ResetToDefault()
{
	OnToggleClicked();
	Super::UpdateWidget();
}

void UToggleSettingBase::OnToggleClicked()
{
	if (bSetByCode) return;
	bCurrentValue = !bCurrentValue;
	PlayAnimation(ToggleAnim, 0.0f, 1, bCurrentValue
		? EUMGSequencePlayMode::Forward : EUMGSequencePlayMode::Reverse);
	SetterFunction(bCurrentValue);
}

void UToggleSettingBase::NativeConstruct()
{
	Super::NativeConstruct();
	ToggleButton->OnClicked.AddUniqueDynamic(this, &UToggleSettingBase::OnToggleClicked);
}

void UToggleSettingBase::NativePreConstruct()
{
	Super::NativePreConstruct();
	bCurrentValue = bDefaultValue;
	SetAnimationCurrentTime(ToggleAnim, bCurrentValue
		? ToggleAnim->GetEndTime() : ToggleAnim->GetStartTime());
}

void USliderSettingBase::UpdateWidget()
{
	bSetByCode = true;
	SliderSpinBox->SetValue(GetterFunction());
	Super::UpdateWidget();
}

bool USliderSettingBase::IsDefaultValue()
{
	return FMath::IsNearlyEqual(SliderSpinBox->GetValue(), DefaultValue);
}

void USliderSettingBase::ResetToDefault()
{
	SliderSpinBox->SetValue(DefaultValue);
	Super::UpdateWidget();
	ApplyChange();
}

void USliderSettingBase::ApplyChange() const
{
	if (bSetByCode) return;
	SetterFunction(SliderSpinBox->GetValue());
}

void USliderSettingBase::OnSliderValueChanged(float Value)
{
	//ApplyChange();
}

void USliderSettingBase::OnSliderMovementEnd(float Value)
{
	ApplyChange();
}

void USliderSettingBase::OnSliderValueCommitted(float Value, ETextCommit::Type Type)
{
	ApplyChange();
}

void USliderSettingBase::NativeConstruct()
{
	Super::NativeConstruct();
	SliderSpinBox->OnValueChanged.AddUniqueDynamic(this, &USliderSettingBase::OnSliderValueChanged);
	SliderSpinBox->OnEndSliderMovement.AddUniqueDynamic(this, &USliderSettingBase::OnSliderMovementEnd);
	SliderSpinBox->OnValueCommitted.AddUniqueDynamic(this, &USliderSettingBase::OnSliderValueCommitted);
}

void USliderSettingBase::NativePreConstruct()
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

void USwitcherSettingBase::UpdateWidget()
{
	bSetByCode = true;
	Value = GetterFunction();
	ValueText->SetText(Options[Value]);
	Super::UpdateWidget();
}

void USwitcherSettingBase::ResetToDefault()
{
	Value = DefaultOption;
	Super::UpdateWidget();
	ApplyChange();
}

void USwitcherSettingBase::ApplyChange() const
{
	SetterFunction(Value);
	ValueText->SetText(Options[Value]);
}

void USwitcherSettingBase::OnPrevClicked()
{
	if (bSetByCode) return;

	Value--;
	if (Value < 0) Value = Options.Num() - 1;
	ApplyChange();
}

void USwitcherSettingBase::OnNextClicked()
{
	if (bSetByCode) return;

	Value++;
	if (Value >= Options.Num()) Value = 0;
	ApplyChange();
}

void USwitcherSettingBase::NativeConstruct()
{
	Super::NativeConstruct();
	PrevButton->OnClicked.AddUniqueDynamic(this, &USwitcherSettingBase::OnPrevClicked);
	NextButton->OnClicked.AddUniqueDynamic(this, &USwitcherSettingBase::OnNextClicked);
}

void USwitcherSettingBase::NativePreConstruct()
{
	Super::NativePreConstruct();
	Value = FMath::Clamp(DefaultOption, 0, Options.Num() - 1);
	ValueText->SetText(Options[Value]);
}

void UComboSettingBase::UpdateWidget()
{
	bSetByCode = true;
	SelectionBox->SetSelectedOption(GetterFunction());
	Super::UpdateWidget();
}

bool UComboSettingBase::IsDefaultValue()
{
	return SelectionBox->GetSelectedOption() == DefaultOption;
}

void UComboSettingBase::ResetToDefault()
{
	SelectionBox->SetSelectedOption(DefaultOption);
	OnSelectionChanged(DefaultOption, ESelectInfo::Type::Direct);
	Super::UpdateWidget();
}

void UComboSettingBase::OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (bSetByCode) return;
	SetterFunction(SelectedItem);
	UpdateWidget();
}

void UComboSettingBase::NativeConstruct()
{
	Super::NativeConstruct();
	SelectionBox->OnSelectionChanged.AddUniqueDynamic(this, &UComboSettingBase::OnSelectionChanged);
}

void UComboSettingBase::NativePreConstruct()
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

void UResolutionSettingBase::UpdateWidget()
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

void UResolutionSettingBase::OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
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

void UResolutionSettingBase::NativePreConstruct()
{
	USettingRowBase::NativePreConstruct();
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

void UImageFidelitySettingBase::UpdateWidget()
{
	bSetByCode = true;
	if (const UToroUserSettings* Settings = UToroUserSettings::Get())
	{
		SelectionBox->SetSelectedOption(ModeToName.FindRef(Settings->GetImageFidelityMode()));
	}
	bSetByCode = false;
}

void UImageFidelitySettingBase::OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (bSetByCode) return;
	if (UToroUserSettings* Settings = UToroUserSettings::Get())
	{
		Settings->SetImageFidelityMode(*ModeToName.FindKey(SelectedItem));
	}
}

void UImageFidelitySettingBase::NativePreConstruct()
{
	USettingRowBase::NativePreConstruct();
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
