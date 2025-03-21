// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroWidget.h"
#include "ToroInterface.h"
#include "GameplayTagContainer.h"
#include "Framework/ToroPlayerCharacter.h"
#include "PauseWidgetBase.generated.h"

class UButton;

UCLASS(Abstract)
class GAMERUNTIME_API UPauseWidgetBase : public UToroWidget, public IToroInterface
{
	GENERATED_BODY()

public:

	UPauseWidgetBase(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> ResumeButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> SettingsButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> CheckpointButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> MainMenuButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<class UTextBlock> GameVersionText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> ScreenFadeAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		TSoftObjectPtr<UWorld> MainMenuLevel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		TSubclassOf<class USettingsWidgetBase> SettingsWidgetClass;

protected:

	UPROPERTY(Transient) TObjectPtr<AToroPlayerCharacter> Player;
	UPROPERTY(Transient) TObjectPtr<USettingsWidgetBase> Settings;

	UFUNCTION() void OnResumeClicked();
	UFUNCTION() void OnSettingsClicked();
	UFUNCTION() void OnCheckpointClicked();
	UFUNCTION() void OnMainMenuClicked();

	void OnPause(const bool bPaused);
	virtual void InitWidget() override;
	virtual void Return_Implementation() override;
	virtual void InternalProcessDeactivation() override;
	virtual bool CanCreateWidget() const override;
};