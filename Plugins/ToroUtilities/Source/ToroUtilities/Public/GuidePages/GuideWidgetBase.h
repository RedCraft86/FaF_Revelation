// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GuidePageTypes.h"
#include "UserWidgets/ToroWidget.h"
#include "Framework/ToroPlayerController.h"
#include "GuideWidgetBase.generated.h"

UCLASS(Abstract)
class TOROUTILITIES_API UGuideWidgetBase : public UToroWidget
{
	GENERATED_BODY()

public:

	UGuideWidgetBase(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<class UButton> NextButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<class UWidgetSwitcher> TypeSwitch;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UPanelWidget> CustomPageContainer;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<class UTextBlock> LocalPageTitle;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<class UExprTextBlock> LocalPageText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<class UImage> LocalPageImage;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<class USizeBox> LocalImageContainer;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> GuideFadeAnim;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> NextButtonAnim;

	UPROPERTY(EditAnywhere, Category = Settings)
		float ImageHeight;

	UPROPERTY(EditAnywhere, Category = Settings)
		TObjectPtr<UTexture2D> DefaultImage;

	void QueuePage(const FGameplayTag& InPageID);
	void QueuePages(const TArray<FGameplayTag>& InPageIDs);

private:

	UPROPERTY() bool bPrePauseState;
	UPROPERTY() FGameInputModeData PreInputMode;
	UPROPERTY(Transient) TObjectPtr<const UGuidePageDatabase> Database;
	TQueue<FGameplayTag> PageQueue;

	void StartBook();
	void ResetBook() const;
	void ProceedNextGuide();
	UFUNCTION() void OnNextClicked();

	virtual void InitWidget() override;
	virtual bool ShouldBeHidden() override;
	virtual void InternalProcessActivation() override;
	virtual void InternalProcessDeactivation() override;
};
