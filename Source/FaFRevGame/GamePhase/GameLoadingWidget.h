// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "UserWidgets/LoadingWidgetBase.h"
#include "GameLoadingWidget.generated.h"

UCLASS(Abstract)
class FAFREVGAME_API UGameLoadingWidget final : public ULoadingWidgetBase
{
	GENERATED_BODY()

public:

	UGameLoadingWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Transient, BlueprintReadOnly, Category = LoadingWidget, meta = (BindWidget))
		TObjectPtr<class UImage> LoadBG;

	UPROPERTY(Transient, BlueprintReadOnly, Category = LoadingWidget, meta = (BindWidget))
		TObjectPtr<class UTextBlock> LoadText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = LoadingWidget, meta = (BindWidget))
		TObjectPtr<class UProgressBar> LoadBar;

	UPROPERTY(EditAnywhere, Category = Settings)
		TObjectPtr<UTexture2D> DefaultBackground;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ForceInlineRow = true, Categories = "Content.Backgrounds"))
		TMap<FGameplayTag, TSoftObjectPtr<UTexture2D>> Backgrounds;

private:

	UPROPERTY() float LoadPercent;
	UPROPERTY() int32 TotalPackages;

	virtual void NativeConstruct() override;
	virtual void InternalProcessActivation() override;
	virtual void InternalProcessDeactivation() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
