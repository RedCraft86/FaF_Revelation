// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroInterface.h"
#include "Blueprint/UserWidget.h"
#include "Framework/ToroPlayerCharacter.h"
#include "MainMenuWidget.generated.h"

class USettingsWidgetBase;
class UButton;

UCLASS(Abstract)
class GAMERUNTIME_API UMainMenuWidgetBase final : public UUserWidget, public IToroInterface
{
	GENERATED_BODY()

public:

	UMainMenuWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

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
	
	UPROPERTY(Transient, BlueprintReadOnly, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> ActivateAnim;
	
	UPROPERTY(Transient, BlueprintReadOnly, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> PlayGameAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		TSoftObjectPtr<UWorld> GameplayLevel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		TSoftObjectPtr<UWorld> ExtrasLevel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		TSubclassOf<USettingsWidgetBase> SettingsWidgetClass;

protected:

	UPROPERTY(Transient) TObjectPtr<AToroPlayerCharacter> Player;
	UPROPERTY(Transient) TObjectPtr<USettingsWidgetBase> SettingsWidget;

	UFUNCTION() void OnPlayClicked();
	UFUNCTION() void OnSettingsClicked();
	UFUNCTION() void OnExtrasClicked();
	UFUNCTION() void OnQuitClicked();

	virtual void NativeConstruct() override;
	virtual void Return_Implementation() override;
};