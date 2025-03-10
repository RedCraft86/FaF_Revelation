// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GeneralInterface.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "MainMenuWidget.generated.h"

class USettingsWidgetBase;
class UButton;

UCLASS(Abstract)
class FAF_REVELATION_API UMainMenuWidgetBase final : public UToroWidget, public IToroGeneralInterface
{
	GENERATED_BODY()

public:

	UMainMenuWidgetBase(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> PlayButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> SettingsButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> ExtrasButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> QuitButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<class UTextBlock> GameVersionText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		TSoftObjectPtr<UWorld> GameplayLevel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		TSoftObjectPtr<UWorld> ExtrasLevel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		TSubclassOf<USettingsWidgetBase> SettingsWidgetClass;

protected:

	UPROPERTY(Transient) TObjectPtr<AToroPlayerBase> PlayerChar;
	UPROPERTY(Transient) TObjectPtr<USettingsWidgetBase> SettingsWidget;

	void OnPlayClicked();
	void OnSettingsClicked();
	void OnExtrasClicked();
	void OnQuitClicked();

	virtual void InitWidget() override;
	virtual void Return_Implementation() override;
};
