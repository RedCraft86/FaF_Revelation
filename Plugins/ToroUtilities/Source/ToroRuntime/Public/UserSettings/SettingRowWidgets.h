// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
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
		TObjectPtr<UButton> LabelButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> ResetButton;

	UPROPERTY(EditAnywhere, Category = Settings)
		FText Label;
	
	UPROPERTY(EditAnywhere, Category = Settings)
		FText Tooltip;

	UPROPERTY(EditAnywhere, Category = Settings)
		ESettingPerformance Performance;

protected:

	UFUNCTION() virtual void OnReset() {}
	UFUNCTION() virtual void OnLabelHover();
	
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
};

UCLASS(Abstract)
class TORORUNTIME_API UToggleSettingBase : public USettingRowBase
{
	GENERATED_BODY()

public:

	UToggleSettingBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
};

UCLASS(Abstract)
class TORORUNTIME_API USliderSettingBase : public USettingRowBase
{
	GENERATED_BODY()

public:

	USliderSettingBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
};

UCLASS(Abstract)
class TORORUNTIME_API USwitcherSettingBase : public USettingRowBase
{
	GENERATED_BODY()

public:

	USwitcherSettingBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
};

UCLASS(Abstract)
class TORORUNTIME_API UComboSettingBase : public USettingRowBase
{
	GENERATED_BODY()

public:

	UComboSettingBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
};