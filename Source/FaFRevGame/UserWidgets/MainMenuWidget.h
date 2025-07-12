// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroUtilities.h"
#include "Interfaces/ExitInterface.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "MainMenuWidget.generated.h"

class UButton;

UCLASS(Abstract)
class FAFREVGAME_API UMainMenuWidget final : public UToroWidgetBase, public IExitInterface
{
	GENERATED_BODY()

public:

	UMainMenuWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> PlayBtn;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> PhaseBtn;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> SettingsBtn;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> ExtrasBtn;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> QuitBtn;

private:

	UPROPERTY(Transient) TObjectPtr<class UPhaseWidget> Phases;
	UPROPERTY(Transient) TObjectPtr<class UExtrasWidget> Extras;
	UPROPERTY(Transient) TObjectPtr<class USettingsWidget> Settings;
	UPROPERTY(Transient) TObjectPtr<class UDifficultyWidget> Difficulty;
	UPROPERTY(Transient) TObjectPtr<class AToroPlayerController> Controller;

	UFUNCTION() void PlayGame();
	UFUNCTION() void PhaseMenu();
	UFUNCTION() void SettingsMenu();
	UFUNCTION() void ExtrasMenu();
	UFUNCTION() void QuitGame();

	void FadeOut(const TFunction<void()>& Callback);

	virtual void InitWidget() override;
	virtual void InternalProcessActivation() override;
	virtual void ReturnToWidget_Implementation(UUserWidget* FromWidget) override;
	virtual bool CanCreateWidget() const override { return UToroUtilSettings::Get()->IsOnLaunchMap(this); }
};
