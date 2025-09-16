// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "UserSettings/ToroUserSettings.h"
#include "UserSettings/UserSettingTypes.h"
#include "SettingRowWidgets.generated.h"

class UButton;
class USpinBox;
class UCheckBox;
class UTextBlock;

#define ADD_DEFAULT_FUNCTIONS(Type) \
	virtual void AssignGetter(const TFunction<Type(UToroUserSettings*)>& Func) { if (Func) GetterFunc = Func; } \
	virtual void AssignSetter(const TFunction<void(UToroUserSettings*, const Type)>& Func) { if (Func) SetterFunc = Func; } \
	virtual void LoadValue() override { \
		bSkipSetter = true; \
		if (GetterFunc) SetValue(GetterFunc(UserSettings)); \
		bSkipSetter = false; \
	}

#define ADD_DEFAULT_PROPERTIES(Type) \
	TFunction<Type(UToroUserSettings*)> GetterFunc; \
	TFunction<void(UToroUserSettings*, const Type)> SetterFunc;

UCLASS(Abstract)
class TOROUTILITIES_API USettingRowTooltip : public UUserWidget
{
	GENERATED_BODY()

public:

	USettingRowTooltip(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer) {}

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> DescText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> ImpactText;

	void UpdateTooltip(const class USettingRowBase* SettingRow) const;
};

UCLASS(Abstract)
class TOROUTILITIES_API USettingRowBase : public UUserWidget
{
	GENERATED_BODY()

public:

	USettingRowBase(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> LabelText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> ResetButton;

	UPROPERTY(EditAnywhere, Category = Settings)
		FText DisplayName;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (MultiLine = true))
		FText Description;

	UPROPERTY(EditAnywhere, Category = Settings)
		ESettingPerformance Performance;

	UPROPERTY(EditAnywhere, Category = Settings)
		TSubclassOf<USettingRowTooltip> TooltipClass;

	DECLARE_MULTICAST_DELEGATE(FOnValueChanged);
	FOnValueChanged OnValueChanged;

	virtual void LoadValue() {}

protected:

	UPROPERTY() bool bSkipSetter;
	UPROPERTY(Transient) TObjectPtr<USettingRowTooltip> Tooltip;
	UPROPERTY(Transient) TObjectPtr<UToroUserSettings> UserSettings;
	
	UFUNCTION() virtual void ResetToDefault() {}
	UFUNCTION() virtual UWidget* CreateTooltip()
	{
		if (!Tooltip) Tooltip = CreateWidget<USettingRowTooltip>(this, TooltipClass);
		Tooltip->UpdateTooltip(this);
		return Tooltip;
	}

	virtual void ApplyValue() {}
	virtual void UpdateWidget();
	virtual bool IsDefaultValue() { return false; }

	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
	virtual void SynchronizeProperties() override
	{
		if (!ToolTipWidgetDelegate.IsBound()) ToolTipWidgetDelegate.BindDynamic(this, &USettingRowBase::CreateTooltip);
		Super::SynchronizeProperties();
	}
};

UCLASS(Abstract)
class TOROUTILITIES_API UToggleSettingRow : public USettingRowBase
{
	GENERATED_BODY()

public:

	UToggleSettingRow(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer), bDefaultValue(false) {}

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UCheckBox> CheckBox;

	UPROPERTY(EditAnywhere, Category = Settings)
		bool bDefaultValue;

	bool GetValue() const;
	void SetValue(const bool InValue);

	ADD_DEFAULT_FUNCTIONS(bool)
	
protected:

	ADD_DEFAULT_PROPERTIES(bool);

	UFUNCTION() void OnStateChanged(bool bIsChecked) { ApplyValue(); }

	virtual void ApplyValue() override;
	virtual void ResetToDefault() override { SetValue(bDefaultValue); }
	virtual bool IsDefaultValue() override { return GetValue() == bDefaultValue; }
	
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
};

UCLASS(Abstract)
class TOROUTILITIES_API USliderSettingRow : public USettingRowBase
{
	GENERATED_BODY()

public:

	USliderSettingRow(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
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

	float GetValue() const;
	void SetValue(const float InValue);

	ADD_DEFAULT_FUNCTIONS(float)
	
protected:

	ADD_DEFAULT_PROPERTIES(float);

	UFUNCTION() void OnSliderEnd(float Value) { ApplyValue(); }
	UFUNCTION() void OnSliderValueChanged(float Value) { ApplyValue(); }
	UFUNCTION() void OnValueCommited(float Value, ETextCommit::Type Type) { ApplyValue(); }

	virtual void ApplyValue() override;
	virtual void ResetToDefault() override { SetValue(DefaultValue); }
	virtual bool IsDefaultValue() override { return FMath::IsNearlyEqual(GetValue(), DefaultValue); }

	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
};

UCLASS(Abstract)
class TOROUTILITIES_API USwapperSettingRow : public USettingRowBase
{
	GENERATED_BODY()

public:

	USwapperSettingRow(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer), DefaultValue(0), Value(0)
	{}

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

	void SetValue(const uint8 InValue);
	uint8 GetValue() const { return Value; }

	ADD_DEFAULT_FUNCTIONS(uint8)
	
protected:

	UPROPERTY() uint8 Value;
	ADD_DEFAULT_PROPERTIES(uint8);

	UFUNCTION() void OnPrevClicked();
	UFUNCTION() void OnNextClicked();

	virtual void ApplyValue() override;
	virtual void UpdateWidget() override;
	virtual void ResetToDefault() override { SetValue(DefaultValue); }
	virtual bool IsDefaultValue() override { return GetValue() == DefaultValue; }
	
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
};

UCLASS(Abstract)
class TOROUTILITIES_API UDropdownSettingRow : public USettingRowBase
{
	GENERATED_BODY()

public:

	UDropdownSettingRow(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer), DefaultValue(TEXT(""))
	{}

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<class UComboBoxString> DropdownBox;

	UPROPERTY(EditAnywhere, Category = Settings)
		TArray<FString> Options;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (GetOptions = Options))
		FString DefaultValue;

	uint8 GetValue() const;
	FString GetValueStr() const;
	void SetValue(const uint8 InValue);
	void SetValueStr(const FString& InValue);

	ADD_DEFAULT_FUNCTIONS(uint8)
	
protected:

	ADD_DEFAULT_PROPERTIES(uint8);

	UFUNCTION() virtual void OnSelection(FString SelectedItem, ESelectInfo::Type SelectionType) { ApplyValue(); }

	virtual void ApplyValue() override;
	virtual void ResetToDefault() override { SetValueStr(DefaultValue); }
	virtual bool IsDefaultValue() override { return GetValueStr() == DefaultValue; }
	
	virtual void FillOptions();
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
};

UCLASS(Abstract)
class TOROUTILITIES_API UResolutionSettingRow : public UDropdownSettingRow
{
	GENERATED_BODY()

public:

	UResolutionSettingRow(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer), LastIdx(0), bReverting(false)
	{}

	void AcceptResolution();
	void RevertResolution();
	
	FIntPoint GetResValue() const;
	void SetResValue(const FIntPoint& InValue);
	virtual void LoadValue() override;

protected:

	UPROPERTY() uint8 LastIdx;
	UPROPERTY() bool bReverting;

	virtual void AssignGetter(const TFunction<uint8(UToroUserSettings*)>& Function) override {}
	virtual void AssignSetter(const TFunction<void(UToroUserSettings*, const uint8)>& Function) override {}
	virtual void OnSelection(FString SelectedItem, ESelectInfo::Type SelectionType) override;
	virtual void FillOptions() override;
#if WITH_EDITOR
	virtual bool CanEditChange(const FProperty* InProperty) const override
	{
		if (InProperty && InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UResolutionSettingRow, Options))
		{
			return false;
		}
		return Super::CanEditChange(InProperty);
	}
#endif
};

UCLASS(Abstract)
class TOROUTILITIES_API UImageFidelitySettingRow : public UDropdownSettingRow
{
	GENERATED_BODY()

public:

	UImageFidelitySettingRow(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer) {}

	EImageFidelityMode GetFidelityMode() const;
	void SetFidelityMode(const EImageFidelityMode InValue);
	virtual void LoadValue() override;

protected:

	static inline TMap<EImageFidelityMode, FString> FidelityModes = {
		{EImageFidelityMode::None, TEXT("None")},
		{EImageFidelityMode::FXAA, TEXT("Fast Approximate AA")},
		{EImageFidelityMode::TAA, TEXT("Temporal AA")},
		{EImageFidelityMode::TSR, TEXT("Temporal Super Resolution")},
		{EImageFidelityMode::FSR, TEXT("FidelityFX Super Resolution 3.1")}
	};

	virtual void AssignGetter(const TFunction<uint8(UToroUserSettings*)>& Function) override {}
	virtual void AssignSetter(const TFunction<void(UToroUserSettings*, const uint8)>& Function) override {}
	virtual void OnSelection(FString SelectedItem, ESelectInfo::Type SelectionType) override;
	virtual void FillOptions() override;
#if WITH_EDITOR
	virtual bool CanEditChange(const FProperty* InProperty) const override
	{
		if (InProperty && InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UImageFidelitySettingRow, Options))
		{
			return false;
		}
		return Super::CanEditChange(InProperty);
	}
#endif
};