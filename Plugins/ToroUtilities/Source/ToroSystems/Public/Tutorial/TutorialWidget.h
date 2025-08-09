// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "DataTypes/InputModeData.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "TutorialWidget.generated.h"

UCLASS(Abstract)
class TOROSYSTEMS_API UTutorialWidget final : public UToroWidgetBase
{
	GENERATED_BODY()

public:

	UTutorialWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<class UButton> NextButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<class UWidgetSwitcher> TypeSwitch;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UPanelWidget> CustomWidgetBox;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<class UTextBlock> TutorialTitle;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<class UExprTextBlock> TutorialText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<class UImage> TutorialImage;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> TutorialAnim;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> NextBtnAnim;

	UPROPERTY(BlueprintReadOnly, Category = Settings)
		TObjectPtr<UTexture2D> DefaultImage;

	UFUNCTION(BlueprintCallable, Category = Subtitles, meta = (WorldContext = "ContextObject"))
		static void QueueTutorial(const UObject* ContextObject, UPARAM(meta = (Categories = "Tutorial")) const FGameplayTag InTutorial, bool bHideKnown);

	UFUNCTION(BlueprintCallable, Category = Subtitles, meta = (WorldContext = "ContextObject"))
		static void QueueTutorials(const UObject* ContextObject, UPARAM(meta = (Categories = "Tutorial")) const TArray<FGameplayTag>& InTutorials, bool bHideKnowns);

private:

	UPROPERTY() bool bLockNextBtn;
	UPROPERTY() FGameInputModeData CachedInput;
	UPROPERTY() TArray<FGameplayTag> TutorialQueue;

	void ShowTutorial();
	UFUNCTION() void OnNextClicked();

	void AddTutorial(const FGameplayTag& InTutorial);
	void AddTutorials(const TArray<FGameplayTag>& InTutorials);

	virtual void InitWidget() override;
	virtual void InternalProcessActivation() override;
	virtual void InternalProcessDeactivation() override;
};
