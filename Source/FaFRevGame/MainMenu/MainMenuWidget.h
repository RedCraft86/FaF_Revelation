// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroRuntime.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ComboBoxString.h"
#include "Interfaces/ExitInterface.h"
#include "UserInterface/ToroManagedWidget.h"
#include "UserSettings/Widgets/SettingsWidget.h"
#include "MainMenuWidget.generated.h"

UCLASS(Abstract)
class FAFREVGAME_API UMainMenuWidget final : public UToroManagedWidget, public IExitInterface
{
	GENERATED_BODY()

public:

	UMainMenuWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = MainMenu)
		void ChangeMenuTheme(UPARAM(meta = (Categories = "MenuTheme")) const FGameplayTag ThemeTag);

	void ShowWidget(class AMainMenuActor* Actor);
	virtual void ReturnToWidget_Implementation(UUserWidget* FromWidget) override;

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> PlayButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> SettingsButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> ExtrasButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> QuitButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UComboBoxString> ThemeDropdown;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> VersionText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidgetAnim))
		TMap<FGameplayTag, FString> Themes;

	FTimerHandle ThemeTimer;
	TObjectPtr<USettingsWidget> Settings;
	TObjectPtr<UDifficultyWidget> Difficulty;
	TObjectPtr<AMainMenuActor> MenuActor;

	UFUNCTION() void OnPlayButton();
	UFUNCTION() void OnSettingsButton();
	UFUNCTION() void OnExtrasButton();
	UFUNCTION() void OnQuitButton();
	UFUNCTION() void OnThemePicked(FString SelectedItem, ESelectInfo::Type SelectionType);

	void OpenGameplayLevel();

	virtual void PushWidget() override;
	virtual void InitWidget(APlayerController* Controller) override;
	virtual bool CanCreateWidget(const UObject* ContextObject) const override
	{
		return UToroSettings::Get()->IsOnMap(ContextObject, EToroMapType::MainMenu);
	}
};
