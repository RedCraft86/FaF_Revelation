// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserWidgets/SettingRowWidgets.h"
#include "UserWidgets/UserDialogWidget.h"
#include "Components/ComboBoxString.h"
#include "Components/TextBlock.h"
#include "Components/CheckBox.h"
#include "Components/SpinBox.h"
#include "Components/Button.h"
#include "EnhancedCodeFlow.h"

void USettingRowTooltip::UpdateTooltip(const USettingRowBase* SettingRow) const
{
	if (SettingRow)
	{
		DescText->SetText(SettingRow->GetToolTipText());
		ImpactText->SetVisibility(ESlateVisibility::HitTestInvisible);
		switch (SettingRow->Performance)
		{
		case ESettingPerformance::Low:
			{
				ImpactText->SetText(INVTEXT("Impact: Low"));
				ImpactText->SetColorAndOpacity(FLinearColor::Green);
			} break;
		case ESettingPerformance::Medium:
			{
				ImpactText->SetText(INVTEXT("Impact: Medium"));
				ImpactText->SetColorAndOpacity(FLinearColor::Yellow);
			} break;
		case ESettingPerformance::High:
			{
				ImpactText->SetText(INVTEXT("Impact: High"));
				ImpactText->SetColorAndOpacity(FLinearColor::Red);
			} break;
		default:
			ImpactText->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

USettingRowBase::USettingRowBase(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
	, DisplayName(INVTEXT("Setting Row")), Performance(ESettingPerformance::None), bSkipSetter(false)
{
	ToolTipWidgetDelegate.BindDynamic(this, &USettingRowBase::CreateTooltip);
}

void USettingRowBase::UpdateWidget()
{
	ResetButton->SetVisibility(IsDefaultValue() ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
}

void USettingRowBase::NativeConstruct()
{
	Super::NativeConstruct();
	UserSettings = UToroUserSettings::Get();
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

// TOGGLE BOX

bool UToggleSettingRow::GetValue() const
{
	return CheckBox->IsChecked();
}

void UToggleSettingRow::SetValue(const bool InValue)
{
	if (GetValue() != InValue)
	{
		CheckBox->SetCheckedState(InValue ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
		UpdateWidget();
		ApplyValue();
	}
}

void UToggleSettingRow::ApplyValue()
{
	if (!bSkipSetter)
	{
		SetterFunc(UserSettings, GetValue());
		OnValueChanged.Broadcast();
	}
}

void UToggleSettingRow::NativeConstruct()
{
	Super::NativeConstruct();
	CheckBox->OnCheckStateChanged.AddUniqueDynamic(this, &UToggleSettingRow::OnStateChanged);
}

void UToggleSettingRow::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (CheckBox) CheckBox->SetIsChecked(bDefaultValue);
}

// SLIDER BOX

float USliderSettingRow::GetValue() const
{
	return SliderBox->GetValue();
}

void USliderSettingRow::SetValue(const float InValue)
{
	if (GetValue() != InValue)
	{
		SliderBox->SetValue(InValue);
		UpdateWidget();
		//ApplyValue();
	}
}

void USliderSettingRow::ApplyValue()
{
	if (!bSkipSetter)
	{
		SetterFunc(UserSettings, SliderBox->GetValue());
		OnValueChanged.Broadcast();
	}
}

void USliderSettingRow::NativeConstruct()
{
	Super::NativeConstruct();
	SliderBox->OnEndSliderMovement.AddDynamic(this, &USliderSettingRow::OnSliderEnd);
	SliderBox->OnValueCommitted.AddDynamic(this, &USliderSettingRow::OnValueCommited);
	if (bAlwaysUpdate) SliderBox->OnValueChanged.AddDynamic(this, &USliderSettingRow::OnSliderValueChanged);
}

void USliderSettingRow::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (SliderBox)
	{
		SliderBox->SetMinValue((float)Range.GetMin());
		SliderBox->SetMaxValue((float)Range.GetMax());
		SliderBox->SetMinSliderValue((float)Range.GetMin());
		SliderBox->SetMaxSliderValue((float)Range.GetMax());
		SliderBox->SetMaxFractionalDigits(NumOfDecimals);
		SliderBox->SetMinFractionalDigits(NumOfDecimals > 0 ? 1 : 0);
		SliderBox->SetDelta(NumOfDecimals > 0 ? 0.0f : 1.0f);
		SliderBox->SetValue(DefaultValue);
	}
}

// SWAPPER BOX

void USwapperSettingRow::SetValue(const uint8 InValue)
{
	if (Value != InValue)
	{
		Value = InValue;
		UpdateWidget();
		ApplyValue();
	}
}

void USwapperSettingRow::OnPrevClicked()
{
	if (Value > 0) SetValue(Value - 1);
	else SetValue(Options.Num() - 1);
}

void USwapperSettingRow::OnNextClicked()
{
	if (Value >= Options.Num() - 1) SetValue(0);
	else SetValue(Value + 1);
}

void USwapperSettingRow::ApplyValue()
{
	if (!bSkipSetter)
	{
		SetterFunc(UserSettings, Value);
		OnValueChanged.Broadcast();
	}
}

void USwapperSettingRow::UpdateWidget()
{
	Super::UpdateWidget();
	if (!Options.IsEmpty())
	{
		OptionText->SetText(Options[Value]);
	}
}

void USwapperSettingRow::NativeConstruct()
{
	Super::NativeConstruct();
	PrevButton->OnClicked.AddUniqueDynamic(this, &USwapperSettingRow::OnPrevClicked);
	NextButton->OnClicked.AddUniqueDynamic(this, &USwapperSettingRow::OnNextClicked);
}

void USwapperSettingRow::NativePreConstruct()
{
	Super::NativePreConstruct();
	Value = FMath::Clamp(DefaultValue, 0, Options.Num() - 1);
	if (OptionText) OptionText->SetText(Options.IsEmpty() ? INVTEXT("INVALID DATA") : Options[Value]);
}

// DROPDOWN BOX

uint8 UDropdownSettingRow::GetValue() const
{
	return DropdownBox->GetSelectedIndex();
}

FString UDropdownSettingRow::GetValueStr() const
{
	return DropdownBox->GetSelectedOption();
}

void UDropdownSettingRow::SetValue(const uint8 InValue)
{
	if (Options.IsValidIndex(InValue) && GetValue() != InValue)
	{
		DropdownBox->SetSelectedIndex(InValue);
		UpdateWidget();
		//ApplyValue();
	}
}

void UDropdownSettingRow::SetValueStr(const FString& InValue)
{
	if (Options.Contains(InValue) && GetValueStr() != InValue)
	{
		DropdownBox->SetSelectedOption(InValue);
		UpdateWidget();
		//ApplyValue();
	}
}

void UDropdownSettingRow::ApplyValue()
{
	if (!bSkipSetter)
	{
		SetterFunc(UserSettings, DropdownBox->GetSelectedIndex());
		OnValueChanged.Broadcast();
	}
}

void UDropdownSettingRow::FillOptions()
{
	DropdownBox->ClearOptions();
	for (const FString& Option : Options)
	{
		DropdownBox->AddOption(Option);
	}
	if (!Options.Contains(DefaultValue))
	{
		DefaultValue = Options.IsEmpty() ? TEXT("INVALID DATA") : Options[0];
	}
	DropdownBox->SetSelectedOption(DefaultValue);
}

void UDropdownSettingRow::NativeConstruct()
{
	Super::NativeConstruct();
	DropdownBox->OnSelectionChanged.AddUniqueDynamic(this, &UDropdownSettingRow::OnSelection);
}

void UDropdownSettingRow::NativePreConstruct()
{
	Super::NativePreConstruct();
	FillOptions();
}

// RESOLUTION BOX

void UResolutionSettingRow::AcceptResolution()
{
	LastIdx = DropdownBox->GetSelectedIndex();
}

void UResolutionSettingRow::RevertResolution()
{
	bReverting = true;
	DropdownBox->SetSelectedIndex(LastIdx);
}

FIntPoint UResolutionSettingRow::GetResValue() const
{
	if (UToroUserSettings::SupportedResolutions.IsValidIndex(GetValue()))
	{
		return UToroUserSettings::SupportedResolutions[GetValue()];
	}
	return UToroUserSettings::FullscreenRes;
}

void UResolutionSettingRow::SetResValue(const FIntPoint& InValue)
{
	UToroUserSettings::CheckSupportedResolutions();
	SetValue(UToroUserSettings::SupportedResolutions.Find(InValue));
}

void UResolutionSettingRow::LoadValue()
{
	bSkipSetter = true;
	SetResValue(UserSettings->GetScreenResolution());
	bSkipSetter = false;
}

void UResolutionSettingRow::OnSelection(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (!bSkipSetter)
	{
		UserSettings->SetScreenResolution(GetResValue());
		UserSettings->SetFullscreenMode(UserSettings->GetScreenResolution() == UToroUserSettings::FullscreenRes ?
#if WITH_EDITOR
			EWindowMode::Type::WindowedFullscreen
#else
			(UserSettings->GetBorderless() ? EWindowMode::Type::WindowedFullscreen : EWindowMode::Type::Fullscreen)
#endif
			: EWindowMode::Type::Windowed);
		UserSettings->ApplyResolutionSettings(false);
		OnValueChanged.Broadcast();

		if (bReverting) bReverting = false;
		else
		{
			UUserDialogWidget::ShowDialog(this, {
				INVTEXT("Resolution Change"),
				INVTEXT("Do you want to keep this resolution?"),
				INVTEXT("Revert"), INVTEXT("Keep")},
				{5.0f,0},
				[WeakThis = TWeakObjectPtr(this)](uint8 Button)
				{
					if (!WeakThis.IsValid()) return;
					if (Button == 0)
					{
						WeakThis->RevertResolution();
					}
					else
					{
						WeakThis->AcceptResolution();
					}
				}
			);
		}
	}
}

void UResolutionSettingRow::FillOptions()
{
	Options.Empty(1);
	DropdownBox->ClearOptions();
	UToroUserSettings::CheckSupportedResolutions();
	for (const FIntPoint& Res : UToroUserSettings::SupportedResolutions)
	{
		const FString AsString(FString::Printf(TEXT("%dx%d"), Res.X, Res.Y));
		DropdownBox->AddOption(AsString);
		Options.Add(AsString);
	}
	if (!Options.Contains(DefaultValue))
	{
		DefaultValue = Options.IsEmpty() ? TEXT("1920x1080") : DropdownBox->GetOptionAtIndex(0);
	}
	DropdownBox->SetSelectedOption(DefaultValue);
}

// IMAGE FIDELITY BOX

EImageFidelityMode UImageFidelitySettingRow::GetFidelityMode() const
{
	const EImageFidelityMode* Mode = ModeToName.FindKey(DropdownBox->GetSelectedOption());
	return Mode && UToroUserSettings::SupportedFidelityModes.Contains(*Mode) ? *Mode : EImageFidelityMode::TAA;
}

void UImageFidelitySettingRow::SetFidelityMode(const EImageFidelityMode InValue)
{
	UToroUserSettings::CheckSupportedFidelityModes();
	if (UToroUserSettings::SupportedFidelityModes.Contains(InValue))
	{
		const FString* Name = ModeToName.Find(InValue);
		SetValueStr(Name ? *Name : ModeToName.FindRef(EImageFidelityMode::TAA));
	}
}

void UImageFidelitySettingRow::LoadValue()
{
	bSkipSetter = true;
	SetFidelityMode(UserSettings->GetImageFidelityMode());
	bSkipSetter = false;
}

void UImageFidelitySettingRow::OnSelection(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (!bSkipSetter)
	{
		UserSettings->SetImageFidelityMode(GetFidelityMode());
		OnValueChanged.Broadcast();
	}
}

void UImageFidelitySettingRow::FillOptions()
{
	Options.Empty(4);
	DropdownBox->ClearOptions();
	UToroUserSettings::CheckSupportedFidelityModes();
	for (const EImageFidelityMode& Mode : UToroUserSettings::SupportedFidelityModes)
	{
		const FString AsString(ModeToName.FindRef(Mode));
		DropdownBox->AddOption(AsString);
		Options.Add(AsString);
	}
	if (!Options.Contains(DefaultValue))
	{
		DefaultValue = ModeToName.FindRef(EImageFidelityMode::TAA);
	}
	DropdownBox->SetSelectedOption(DefaultValue);
}
