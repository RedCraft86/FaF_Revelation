// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "LoadingWidgetBase.generated.h"

UCLASS(Abstract)
class FAFREVGAME_API ULoadingWidgetBase final : public UToroWidgetBase
{
	GENERATED_BODY()

public:

	ULoadingWidgetBase(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Transient, BlueprintReadOnly, Category = LoadingWidget, meta = (BindWidget))
		TObjectPtr<UTextBlock> LoadText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = LoadingWidget, meta = (BindWidget))
		TObjectPtr<UProgressBar> LoadBar;

private:

	UPROPERTY() float LoadPercent;
	UPROPERTY() int32 TotalPackages;

	virtual void InternalProcessActivation() override;
	virtual void InternalProcessDeactivation() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
