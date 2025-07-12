// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroUtilities.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "DifficultyWidget.generated.h"

UCLASS(Abstract)
class TOROUTILITIES_API UDifficultyWidget final : public UToroWidgetBase
{
	GENERATED_BODY()

public:

	UDifficultyWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> ReloadInfo;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> EasyButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UWidget> EasySelectVis;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> NormalButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UWidget> NormalSelectVis;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> HardButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UWidget> HardSelectVis;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> ConfirmButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Settings)
		TObjectPtr<UUserWidget> ParentUI;

protected:

	UPROPERTY() EGameDifficulty Difficulty;

	UFUNCTION() void OnConfirmClicked();
	UFUNCTION() void OnEasyClicked() { SetDifficulty(EGameDifficulty::Easy); }
	UFUNCTION() void OnNormalClicked() { SetDifficulty(EGameDifficulty::Normal); }
	UFUNCTION() void OnHardClicked() { SetDifficulty(EGameDifficulty::Hard); }
	void SetDifficulty(const EGameDifficulty InDifficulty);

	virtual void InitWidget() override;
	virtual void InternalProcessActivation() override;
};
