// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserSettings/SettingRowWidgets.h"
#include "Animation/WidgetAnimation.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "EnhancedCodeFlow.h"
#include "Components/ComboBoxString.h"
#include "Components/SpinBox.h"
#include "UserSettings/ToroUserSettings.h"

#define BEGIN_SKIP_SETTER bSkipSetter = true;
#define END_SKIP_SETTER bSkipSetter = false;
#define DEFINE_LOAD_VALUE \
	BEGIN_SKIP_SETTER \
	SetValue(GetterFunc()); \
	END_SKIP_SETTER

void USettingTooltipBase::UpdateTooltip(class USettingRowBase* SettingRow) const
{
	if (SettingRow)
	{
		DescText->SetText(SettingRow->GetToolTipText());
		PerformanceText->SetVisibility(ESlateVisibility::HitTestInvisible);
		switch (SettingRow->Performance)
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

void USettingRowBase::UpdateWidget()
{
	ResetButton->SetIsEnabled(!IsDefaultValue());
	ResetButton->SetVisibility(ResetButton->GetIsEnabled() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void USettingRowBase::NativeConstruct()
{
	Super::NativeConstruct();
	ToroSettings = UToroUserSettings::Get();
	LabelText->SetVisibility(ESlateVisibility::Visible);
	ResetButton->OnClicked.AddUniqueDynamic(this, &USettingRowBase::ResetToDefault);
	FFlow::DelayTicks(this, 1, [this]()
	{
		LoadValue();
	});
}

void USettingRowBase::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (LabelText) LabelText->SetText(DisplayName);
}

void UToggleRowBase::SetValue(const bool InValue)
{
	if (bCurrentValue != InValue)
	{
		bCurrentValue = InValue;
		UpdateWidget();
		if (!bSkipSetter)
		{
			SetterFunc(InValue);
			OnValueChanged.Broadcast(InValue);
		}
	}
}

void UToggleRowBase::LoadValue()
{
	DEFINE_LOAD_VALUE
}

void UToggleRowBase::UpdateWidget()
{
	Super::UpdateWidget();
	PlayAnimation(ToggleAnim, 0.0f, 1, bCurrentValue
		? EUMGSequencePlayMode::Forward : EUMGSequencePlayMode::Reverse);
}

void UToggleRowBase::NativeConstruct()
{
	Super::NativeConstruct();
	ToggleButton->OnClicked.AddUniqueDynamic(this, &UToggleRowBase::OnToggleClicked);
}

void UToggleRowBase::NativePreConstruct()
{
	Super::NativePreConstruct();
	bCurrentValue = bDefaultValue;
	if (ToggleAnim) SetAnimationCurrentTime(ToggleAnim, bCurrentValue
		? ToggleAnim->GetEndTime() : ToggleAnim->GetStartTime());
}

void USliderRowBase::RunSetter()
{
	UpdateWidget();
	if (!bSkipSetter)
	{
		SetterFunc(SliderBox->GetValue());
		OnValueChanged.Broadcast(SliderBox->GetValue());
	}
}

void USliderRowBase::SetValue(const float InValue)
{
	if (!FMath::IsNearlyEqual(SliderBox->GetValue(), DefaultValue))
	{
		SliderBox->SetValue(InValue);
		RunSetter();
	}
}

void USliderRowBase::LoadValue()
{
	DEFINE_LOAD_VALUE
}

bool USliderRowBase::IsDefaultValue()
{
	return FMath::IsNearlyEqual(SliderBox->GetValue(), DefaultValue);
}

void USliderRowBase::ApplySliderSettings() const
{
	SliderBox->SetMinValue(Range.GetMin());
	SliderBox->SetMaxValue(Range.GetMax());
	SliderBox->SetMinSliderValue(Range.GetMin());
	SliderBox->SetMaxSliderValue(Range.GetMax());
	SliderBox->SetMaxFractionalDigits(NumOfDecimals);
	SliderBox->SetMinFractionalDigits(NumOfDecimals > 0 ? 1 : 0);
	SliderBox->SetDelta(NumOfDecimals > 0 ? 0.0f : 1.0f);
}

void USliderRowBase::NativeConstruct()
{
	Super::NativeConstruct();
	SliderBox->OnEndSliderMovement.AddDynamic(this, &USliderRowBase::OnSliderEnd);
	SliderBox->OnValueCommitted.AddDynamic(this, &USliderRowBase::OnValueCommited);
}

void USliderRowBase::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (SliderBox)
	{
		SliderBox->SetValue(FMath::Clamp(DefaultValue, Range.GetMin(), Range.GetMax()));
		ApplySliderSettings();
	}
}

void USwapperRowBase::SetValue(const uint8 InValue)
{
	if (Value != InValue)
	{
		Value = InValue;
		UpdateWidget();
		if (!bSkipSetter)
		{
			SetterFunc(Value);
			OnValueChanged.Broadcast(InValue);
		}
	}
}

void USwapperRowBase::OnPrevClicked()
{
	if (Value > 0) SetValue(Value - 1);
	else SetValue(Options.Num() - 1);
}

void USwapperRowBase::OnNextClicked()
{
	if (Value >= Options.Num() - 1) SetValue(0);
	else SetValue(Value + 1);
}

void USwapperRowBase::LoadValue()
{
	DEFINE_LOAD_VALUE
}

void USwapperRowBase::UpdateWidget()
{
	Super::UpdateWidget();
	if (!Options.IsEmpty())
	{
		OptionText->SetText(Options[Value]);
	}
}

void USwapperRowBase::NativeConstruct()
{
	Super::NativeConstruct();
	PrevButton->OnClicked.AddUniqueDynamic(this, &USwapperRowBase::OnPrevClicked);
	NextButton->OnClicked.AddUniqueDynamic(this, &USwapperRowBase::OnNextClicked);
}

void USwapperRowBase::NativePreConstruct()
{
	Super::NativePreConstruct();
	Value = FMath::Clamp(DefaultValue, 0, Options.Num() - 1);
	if (!Options.IsEmpty() && OptionText) OptionText->SetText(Options[Value]);
}

void USelectorRowBase::SetValueIdx(const uint8 InValue) const
{
	if (Options.IsEmpty()) return;
	if (SelectionBox->GetSelectedIndex() != InValue)
	{
		SelectionBox->SetSelectedIndex(FMath::Clamp(InValue, 0, Options.Num() - 1));
	}
}

void USelectorRowBase::SetValueStr(const FString& InValue) const
{
	if (Options.IsEmpty()) return;
	if (SelectionBox->GetSelectedOption() != InValue)
	{
		SelectionBox->SetSelectedOption(InValue);
	}
}

void USelectorRowBase::OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	UpdateWidget();
	if (!bSkipSetter)
	{
		SetterFunc(SelectionBox->GetSelectedOption());
		OnValueChanged.Broadcast(SelectedItem);
	}
}

bool USelectorRowBase::IsDefaultValue()
{
	return SelectionBox->GetSelectedOption() == DefaultValue;
}

void USelectorRowBase::GenerateOptions()
{
	SelectionBox->ClearOptions();
	for (const FString& Option : Options)
	{
		SelectionBox->AddOption(Option);
	}
	if (!Options.Contains(DefaultValue))
	{
		DefaultValue = Options.IsEmpty() ? TEXT("") : Options[0];
		SelectionBox->SetSelectedOption(DefaultValue);
	}
}

void USelectorRowBase::NativeConstruct()
{
	Super::NativeConstruct();
	SelectionBox->OnSelectionChanged.AddUniqueDynamic(this, &USelectorRowBase::OnSelectionChanged);
}

void USelectorRowBase::NativePreConstruct()
{
	Super::NativePreConstruct();
	GenerateOptions();
}

void UResolutionRowBase::SetValue(const FIntPoint& InValue) const
{
	UToroUserSettings::CheckSupportedResolutions();
	SetValueIdx(UToroUserSettings::SupportedResolutions.Find(InValue));
}

void UResolutionRowBase::LoadValue()
{
	BEGIN_SKIP_SETTER
	SetValue(ToroSettings->GetScreenResolution());
	END_SKIP_SETTER
}

void UResolutionRowBase::OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	UpdateWidget();
	if (!bSkipSetter)
	{
		FString StrX, StrY;
		SelectedItem.Split(TEXT("x"), &StrX, &StrY);
		const FIntPoint Res{FCString::Atoi(*StrX), FCString::Atoi(*StrY)};

		ToroSettings->SetScreenResolution(Res);
		ToroSettings->SetFullscreenMode(Res == UToroUserSettings::FullscreenRes ?
#if WITH_EDITOR
			EWindowMode::Type::WindowedFullscreen
#else
			(ToroSettings->GetBorderless() ? EWindowMode::Type::WindowedFullscreen : EWindowMode::Type::Fullscreen)
#endif
			: EWindowMode::Type::Windowed);
		ToroSettings->ApplyResolutionSettings(false);
		OnValueChanged.Broadcast(SelectedItem);
	}
}

void UResolutionRowBase::GenerateOptions()
{
	SelectionBox->ClearOptions();
	UToroUserSettings::CheckSupportedResolutions();
	for (const FIntPoint& Res : UToroUserSettings::SupportedResolutions)
	{
		SelectionBox->AddOption(FString::Printf(TEXT("%dx%d"), Res.X, Res.Y));
	}
	if (!Options.Contains(DefaultValue))
	{
		DefaultValue = SelectionBox->GetOptionAtIndex(0);
		SelectionBox->SetSelectedOption(DefaultValue);
	}
}

void UImageFidelityBase::SetValue(const EImageFidelityMode& InValue) const
{
	UToroUserSettings::CheckSupportedFidelityModes();
	if (UToroUserSettings::SupportedFidelityModes.Contains(InValue))
	{
		const FString* Name = ModeToName.Find(InValue);
		Name ? SetValueStr(*Name) : SetValue(EImageFidelityMode::TAA);
	}
}

void UImageFidelityBase::LoadValue()
{
	BEGIN_SKIP_SETTER
	SetValue(ToroSettings->GetImageFidelityMode());
	END_SKIP_SETTER
}

void UImageFidelityBase::OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	UpdateWidget();
	if (!bSkipSetter)
	{
		const EImageFidelityMode* Mode = ModeToName.FindKey(SelectedItem);
		ToroSettings->SetImageFidelityMode(Mode ? *Mode : EImageFidelityMode::TAA);
		OnValueChanged.Broadcast(SelectedItem);
	}
}

void UImageFidelityBase::GenerateOptions()
{
	SelectionBox->ClearOptions();
	UToroUserSettings::CheckSupportedFidelityModes();
	for (const EImageFidelityMode& Mode : UToroUserSettings::SupportedFidelityModes)
	{
		SelectionBox->AddOption(ModeToName.FindRef(Mode));
	}
	if (!Options.Contains(DefaultValue))
	{
		DefaultValue = ModeToName.FindRef(EImageFidelityMode::TAA);
		SelectionBox->SetSelectedOption(DefaultValue);
	}
}
