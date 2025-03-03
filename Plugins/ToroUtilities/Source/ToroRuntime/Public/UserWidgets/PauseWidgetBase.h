// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroWidgetBase.h"
#include "GeneralInterface.h"
#include "ToroAnimatedButtonBase.h"
#include "PauseWidgetBase.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API UPauseWidgetBase : public UToroWidget, public IToroGeneralInterface
{
	GENERATED_BODY()

public:

	UPauseWidgetBase(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UToroAnimatedButtonBase> ResumeButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UToroAnimatedButtonBase> SettingsButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UToroAnimatedButtonBase> CheckpointButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UToroAnimatedButtonBase> MainMenuButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<class UTextBlock> GameVersionText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> ScreenFadeAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		TSoftObjectPtr<UWorld> MainMenuLevel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		TSubclassOf<class USettingsWidgetBase> SettingsWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (Categories = "Saves"))
		FGameplayTag SaveTag;

protected:

	UPROPERTY(Transient) TObjectPtr<class AToroPlayerBase> PlayerChar;
	UPROPERTY(Transient) TObjectPtr<USettingsWidgetBase> SettingsWidget;

	void OnResumeClicked();
	void OnSettingsClicked();
	void OnCheckpointClicked();
	void OnMainMenuClicked();

	virtual void InitWidget() override;
	virtual void Return_Implementation() override;
	virtual void InternalProcessDeactivation() override;
};
