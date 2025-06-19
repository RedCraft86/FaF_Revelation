// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroSettings.h"
#include "DataTypes/InputModeData.h"
#include "Interfaces/ExitInterface.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "GamePauseWidget.generated.h"

class UButton;

UCLASS(Abstract)
class TOROUTILITIES_API UGamePauseWidget : public UToroWidgetBase, public IExitInterface
{
	GENERATED_BODY()

public:

	UGamePauseWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> ResumeBtn;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> SettingsBtn;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> CheckpointBtn;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> MainMenuBtn;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> QuitBtn;

protected:

	UPROPERTY() FGameInputModeData CachedInput;
	UPROPERTY(Transient) TObjectPtr<class UGameSettingsWidget> Settings;
	UPROPERTY(Transient) TObjectPtr<class AToroPlayerController> Controller;

	UFUNCTION() void ResumeGame();
	UFUNCTION() void SettingsMenu();
	UFUNCTION() void Checkpoint();
	UFUNCTION() void MainMenu();
	UFUNCTION() void QuitGame();

	void OnGamePause(const bool bPaused);
	void FadeOut(const TFunction<void()>& Callback);

	virtual void InitWidget() override;
	virtual void InternalProcessActivation() override;
	virtual void ReturnToWidget_Implementation(UUserWidget* FromWidget) override;
	virtual bool CanCreateWidget() const override { return UToroSettings::Get()->IsOnGameplayMap(this); }
};
