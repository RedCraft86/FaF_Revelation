// Copyright (C) RedCraft86. All Rights Reserved.

// ReSharper disable CppMemberFunctionMayBeConst
#pragma once

#include "UserSettingTypes.h"
#include "Blueprint/UserWidget.h"
#include "SettingRowWidgets.generated.h"

class UToroUserSettings;
class UButton;
class USpinBox;
class UTextBlock;

UCLASS(Abstract)
class TORORUNTIME_API USettingTooltipBase : public UUserWidget
{
	GENERATED_BODY()

public:

	USettingTooltipBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> DescText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> PerformanceText;

	void UpdateTooltip(class USettingRowBase* SettingRow) const;
};

UCLASS(Abstract)
class TORORUNTIME_API USettingRowBase : public UUserWidget
{
	GENERATED_BODY()

public:

	USettingRowBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
		, DisplayName(NSLOCTEXT("Toro", "SettingRowName", "Setting Row"))
		, Performance(ESettingPerformance::None), bSkipSetter(false)
	{
		ToolTipWidgetDelegate.BindDynamic(this, &USettingRowBase::CreateTooltip);
	}

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> LabelText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> ResetButton;

	UPROPERTY(EditAnywhere, Category = Settings)
		FText DisplayName;

	UPROPERTY(EditAnywhere, Category = Settings)
		FText Description;

	UPROPERTY(EditAnywhere, Category = Settings)
		ESettingPerformance Performance;

	UPROPERTY(EditAnywhere, Category = Settings)
		TSubclassOf<USettingTooltipBase> TooltipClass;

	virtual void LoadValue() {}

protected:

	UPROPERTY() bool bSkipSetter;
	UPROPERTY(Transient) TObjectPtr<UToroUserSettings> ToroSettings;
	virtual void UpdateWidget();
	virtual bool IsDefaultValue() { return false; }
	UFUNCTION() virtual void ResetToDefault() {}
	UFUNCTION() virtual UWidget* CreateTooltip()
	{
		USettingTooltipBase* Widget = CreateWidget<USettingTooltipBase>(this, TooltipClass);
		Widget->UpdateTooltip(this);
		return Widget;
	}

	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
	virtual void SynchronizeProperties() override
	{
		if (!ToolTipWidgetDelegate.IsBound()) ToolTipWidgetDelegate.BindDynamic(this, &USettingRowBase::CreateTooltip);
		Super::SynchronizeProperties();
	}
};

UCLASS(Abstract)
class TORORUNTIME_API UToggleRowBase : public USettingRowBase
{
	GENERATED_BODY()

public:

	UToggleRowBase(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer), bDefaultValue(false), bCurrentValue(false)
	{}

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> ToggleButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> ToggleAnim;

	UPROPERTY(EditAnywhere, Category = Settings)
		bool bDefaultValue;

	DECLARE_MULTICAST_DELEGATE(FOnValueChanged);
	FOnValueChanged OnValueChanged;

	virtual void LoadValue() override;
	bool GetValue() const { return bCurrentValue; }
	virtual void AssignGetter(const TFunction<bool()>& Function) { if (Function) GetterFunc = Function; }
	virtual void AssignSetter(const TFunction<void(const bool)>& Function) { if (Function) SetterFunc = Function; }

protected:

	UPROPERTY() bool bCurrentValue;
	TFunction<bool()> GetterFunc;
	TFunction<void(const bool)> SetterFunc;

	void SetValue(const bool InValue);
	UFUNCTION() void OnToggleClicked() { SetValue(!bCurrentValue); }
	virtual void UpdateWidget() override;
	virtual void ResetToDefault() override { SetValue(bDefaultValue); }
	virtual bool IsDefaultValue() override { return bCurrentValue != bDefaultValue; }

	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
};

UCLASS(Abstract)
class TORORUNTIME_API USliderRowBase : public USettingRowBase
{
	GENERATED_BODY()

public:

	USliderRowBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
		, DefaultValue(0.0f), Range(0.0f, 100.0f), NumOfDecimals(0), bAlwaysUpdate(false)
	{}

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<USpinBox> SliderBox;

	UPROPERTY(EditAnywhere, Category = Settings)
		float DefaultValue;

	UPROPERTY(EditAnywhere, Category = Settings)
		FVector2D Range;

	UPROPERTY(EditAnywhere, Category = Settings)
		uint8 NumOfDecimals;

	UPROPERTY(EditAnywhere, Category = Settings)
		bool bAlwaysUpdate;

	DECLARE_MULTICAST_DELEGATE(FOnValueChanged);
	FOnValueChanged OnValueChanged;

	float GetValue() const;
	virtual void LoadValue() override;
	virtual void AssignGetter(const TFunction<float()>& Function) { if (Function) GetterFunc = Function; }
	virtual void AssignSetter(const TFunction<void(const float)>& Function) { if (Function) SetterFunc = Function; }

protected:

	TFunction<float()> GetterFunc;
	TFunction<void(const float)> SetterFunc;

	void RunSetter();
	void SetValue(const float InValue);
	virtual bool IsDefaultValue() override;
	virtual void ResetToDefault() override { SetValue(DefaultValue); }
	UFUNCTION() void OnValueCommited(float Value, ETextCommit::Type Type) { RunSetter(); }
	UFUNCTION() void OnSliderValueChanged(float Value) { if (bAlwaysUpdate) RunSetter(); }
	UFUNCTION() void OnSliderEnd(float Value) { RunSetter(); }


	void ApplySliderSettings() const;
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
};

UCLASS(Abstract)
class TORORUNTIME_API USwapperRowBase : public USettingRowBase
{
	GENERATED_BODY()

public:
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> OptionText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> PrevButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> NextButton;

	UPROPERTY(EditAnywhere, Category = Settings)
		TArray<FText> Options;

	UPROPERTY(EditAnywhere, Category = Settings)
		uint8 DefaultValue;

	DECLARE_MULTICAST_DELEGATE(FOnValueChanged);
	FOnValueChanged OnValueChanged;

	virtual void LoadValue() override;
	uint8 GetValue() const { return Value; }
	virtual void AssignGetter(const TFunction<uint8()>& Function) { if (Function) GetterFunc = Function; }
	virtual void AssignSetter(const TFunction<void(const uint8)>& Function) { if (Function) SetterFunc = Function; }

protected:

	UPROPERTY() uint8 Value;
	TFunction<uint8()> GetterFunc;
	TFunction<void(const uint8)> SetterFunc;

	void SetValue(const uint8 InValue);
	UFUNCTION() void OnPrevClicked();
	UFUNCTION() void OnNextClicked();

	virtual void UpdateWidget() override;
	virtual void ResetToDefault() override { SetValue(DefaultValue); }
	virtual bool IsDefaultValue() override { return Value != DefaultValue; }

	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
};

UCLASS(Abstract)
class TORORUNTIME_API USelectorRowBase : public USettingRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<class UComboBoxString> SelectionBox;

	UPROPERTY(EditAnywhere, Category = Settings)
		TArray<FString> Options;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (GetOptions = Options))
		FString DefaultValue;

	DECLARE_MULTICAST_DELEGATE(FOnValueChanged);
	FOnValueChanged OnValueChanged;

	FString GetValue() const;
	virtual void LoadValue() override { checkf(0, TEXT("Not Implemented")); }
	virtual void AssignGetter(const TFunction<FString()>& Function) { if (Function) GetterFunc = Function; }
	virtual void AssignSetter(const TFunction<void(const FString)>& Function) { if (Function) SetterFunc = Function; }

protected:

	TFunction<FString()> GetterFunc;
	TFunction<void(const FString)> SetterFunc;

	void SetValueIdx(const uint8 InValue) const;
	void SetValueStr(const FString& InValue) const;
	UFUNCTION() virtual void OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	virtual bool IsDefaultValue() override;
	virtual void ResetToDefault() override { SetValueStr(DefaultValue); }

	virtual void GenerateOptions();
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
};

UCLASS(Abstract)
class TORORUNTIME_API UResolutionRowBase : public USelectorRowBase
{
	GENERATED_BODY()

public:

	UResolutionRowBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), LastIdx(0) {}

	void AcceptResolution();
	void RevertResolution() const;
	virtual void LoadValue() override;

protected:

	UPROPERTY() uint8 LastIdx;
	void SetValue(const FIntPoint& InValue) const;
	virtual void AssignGetter(const TFunction<FString()>& Function) override { }
	virtual void AssignSetter(const TFunction<void(const FString)>& Function) override { }

	virtual void OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType) override;
	virtual void GenerateOptions() override;
};

UCLASS(Abstract)
class TORORUNTIME_API UImageFidelityBase : public USelectorRowBase
{
	GENERATED_BODY()

public:

	UImageFidelityBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

	virtual void LoadValue() override;

protected:

	static inline TMap<EImageFidelityMode, FString> ModeToName = {
		{EImageFidelityMode::None, TEXT("None")},
		{EImageFidelityMode::FXAA, TEXT("FXAA")},
		{EImageFidelityMode::TAA, TEXT("TAA")},
		{EImageFidelityMode::SMAA, TEXT("SMAA")},
		{EImageFidelityMode::TSR, TEXT("TSR")},
		{EImageFidelityMode::DLSS, TEXT("DLSS")},
		{EImageFidelityMode::FSR, TEXT("FSR")},
		{EImageFidelityMode::XeSS, TEXT("XeSS")}
	};

	void SetValue(const EImageFidelityMode& InValue) const;
	virtual void AssignGetter(const TFunction<FString()>& Function) override { }
	virtual void AssignSetter(const TFunction<void(const FString)>& Function) override { }

	virtual void OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType) override;
	virtual void GenerateOptions() override;
};
