// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Components/ComboBoxString.h"
#include "DataTypes/UserSettingTypes.h"
#include "SettingRowWidgets.generated.h"

UENUM(BlueprintType)
enum class ESettingPerformance : uint8
{
	None,
	Low,
	Medium,
	High
};

UCLASS(Abstract, NotBlueprintable)
class TORORUNTIME_API USettingTooltipBase : public UUserWidget
{
	GENERATED_BODY()

public:

	USettingTooltipBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> DescText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> PerformanceText;

	void UpdateTooltip(const class USettingRowBase* InRow) const;
};

UCLASS(Abstract, NotBlueprintable)
class TORORUNTIME_API USettingRowBase : public UUserWidget
{
	GENERATED_BODY()

public:

	USettingRowBase(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer), Performance(ESettingPerformance::None)
	{}

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> LabelText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> ResetButton;

	UPROPERTY(EditAnywhere, Category = Settings)
		FText Label;

	UPROPERTY(EditAnywhere, Category = Settings)
		FText Tooltip;

	UPROPERTY(EditAnywhere, Category = Settings)
		ESettingPerformance Performance;

	UPROPERTY(EditAnywhere, Category = Settings)
		TSubclassOf<USettingTooltipBase> TooltipWidgetClass;

	virtual void UpdateWidget();
	virtual bool IsDefaultValue() { return false; }
	UFUNCTION() virtual void ResetToDefault() {}

protected:

	UPROPERTY() bool bSetByCode = false;
	UFUNCTION() virtual UWidget* CreateTooltip();

	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
	virtual void SynchronizeProperties() override;
};

UCLASS(Abstract)
class TORORUNTIME_API UToggleSettingBase : public USettingRowBase
{
	GENERATED_BODY()

public:

	UToggleSettingBase(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer), bDefaultValue(false), bCurrentValue(false)
	{}

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> ToggleButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> ToggleAnim;

	UPROPERTY(EditAnywhere, Category = Settings)
		bool bDefaultValue;

	virtual void UpdateWidget() override;
	virtual void ResetToDefault() override;
	virtual bool IsDefaultValue() override { return bCurrentValue != bDefaultValue; }
	virtual void AssignGetterFunction(const TFunction<bool()>& Function) { if (Function) GetterFunction = Function; }
	virtual void AssignSetterFunction(const TFunction<void(const bool)>& Function) { if (Function) SetterFunction = Function; }

protected:

	UPROPERTY() bool bCurrentValue;
	UFUNCTION() void OnToggleClicked();

	TFunction<bool()> GetterFunction;
	TFunction<void(const bool)> SetterFunction;

	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
};

UCLASS(Abstract)
class TORORUNTIME_API USliderSettingBase : public USettingRowBase
{
	GENERATED_BODY()

public:

	USliderSettingBase(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer), DefaultValue(0.0f), Range(0.0f, 100.0f), NumOfDecimals(0)
	{}

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<class USpinBox> SliderSpinBox;

	UPROPERTY(EditAnywhere, Category = Settings)
		float DefaultValue;

	UPROPERTY(EditAnywhere, Category = Settings)
		FVector2D Range;

	UPROPERTY(EditAnywhere, Category = Settings)
		uint8 NumOfDecimals;

	virtual void UpdateWidget() override;
	virtual bool IsDefaultValue() override;
	virtual void ResetToDefault() override;
	virtual void AssignGetterFunction(const TFunction<float()>& Function) { if (Function) GetterFunction = Function; }
	virtual void AssignSetterFunction(const TFunction<void(const float)>& Function) { if (Function) SetterFunction = Function; }

protected:
	
	TFunction<float()> GetterFunction;
	TFunction<void(const float)> SetterFunction;

	void ApplyChange() const;
	UFUNCTION() void OnSliderValueChanged(float Value);
	UFUNCTION() void OnSliderMovementEnd(float Value);
	UFUNCTION() void OnSliderValueCommitted(float Value, ETextCommit::Type Type);

	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
};

UCLASS(Abstract)
class TORORUNTIME_API USwitcherSettingBase : public USettingRowBase
{
	GENERATED_BODY()

public:

	USwitcherSettingBase(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer), DefaultOption(0), Value(0)
	{}

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> ValueText;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> PrevButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> NextButton;
	
	UPROPERTY(EditAnywhere, Category = Settings, meta = (NoElementDuplicate))
		TArray<FText> Options;

	UPROPERTY(EditAnywhere, Category = Settings)
		uint8 DefaultOption;

	virtual void UpdateWidget() override;
	virtual void ResetToDefault() override;
	virtual bool IsDefaultValue() override { return Value != DefaultOption; }
	virtual void AssignGetterFunction(const TFunction<uint8()>& Function) { if (Function) GetterFunction = Function; }
	virtual void AssignSetterFunction(const TFunction<void(const uint8)>& Function) { if (Function) SetterFunction = Function; }

protected:

	UPROPERTY() int32 Value;
	TFunction<uint8()> GetterFunction;
	TFunction<void(const uint8)> SetterFunction;

	void ApplyChange() const;
	UFUNCTION() void OnPrevClicked();
	UFUNCTION() void OnNextClicked();

	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
};

UCLASS(Abstract)
class TORORUNTIME_API UComboSettingBase : public USettingRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UComboBoxString> SelectionBox;

	UPROPERTY(EditAnywhere, Category = Settings)
		TArray<FString> Options;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (GetOptions = Options))
		FString DefaultOption;

	UComboSettingBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

	virtual void UpdateWidget() override;
	virtual bool IsDefaultValue() override;
	virtual void ResetToDefault() override;
	virtual void AssignGetterFunction(const TFunction<FString()>& Function) { if (Function) GetterFunction = Function; }
	virtual void AssignSetterFunction(const TFunction<void(const FString)>& Function) { if (Function) SetterFunction = Function; }

protected:
	
	TFunction<FString()> GetterFunction;
	TFunction<void(const FString)> SetterFunction;
	UFUNCTION() virtual void OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
};

UCLASS(Abstract)
class TORORUNTIME_API UResolutionSettingBase : public UComboSettingBase
{
	GENERATED_BODY()

public:

	UResolutionSettingBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnResolutionChanged, const FString&);
	FOnResolutionChanged OnResolutionChanged;

	virtual void UpdateWidget() override;
	virtual bool IsDefaultValue() override { return true; }

protected:

	UPROPERTY() FString Value;

	virtual void AssignGetterFunction(const TFunction<FString()>& Function) override {}
	virtual void AssignSetterFunction(const TFunction<void(const FString)>& Function) override {}
	virtual void OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType) override;
	virtual void NativePreConstruct() override;
};

UCLASS(Abstract)
class TORORUNTIME_API UImageFidelitySettingBase : public UComboSettingBase
{
	GENERATED_BODY()

public:

	UImageFidelitySettingBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

	virtual void UpdateWidget() override;
	virtual bool IsDefaultValue() override { return true; }

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

	virtual void AssignGetterFunction(const TFunction<FString()>& Function) override {}
	virtual void AssignSetterFunction(const TFunction<void(const FString)>& Function) override {}
	virtual void OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType) override;
	virtual void NativePreConstruct() override;
};
