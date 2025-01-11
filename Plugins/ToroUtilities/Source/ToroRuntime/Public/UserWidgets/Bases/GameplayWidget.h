// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserWidgets/ToroUserWidget.h"
#include "GameplayWidget.generated.h"

class UImage;
class UExprTextBlock;

UCLASS(Abstract)
class TORORUNTIME_API UGameplayWidgetBase final : public UToroUserWidget
{
	GENERATED_BODY()

	friend class UInteractionComponent;

public:

	UGameplayWidgetBase(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintCallable, Category = UserWidget)
		void SetInteractionHidden(const bool bInHidden);
	
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateProgress(const float Progress);
	
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateLabelPos(UExprTextBlock* LabelObj, const FVector2D& BaseOffset, const float IconSizeRatio);
	
private:

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UImage> BaseIcon;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UImage> CustomIcon;
	
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UExprTextBlock> Label;
	
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UWidget> ProgressWidget;
	
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UPanelWidget> InteractWrapper;
	
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UProgressBar> StaminaBar;
	
	UPROPERTY(meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> InteractHideAnim;
	
	UPROPERTY(EditAnywhere, Category = Settings)
		TObjectPtr<UTexture2D> DefaultIcon;

	UPROPERTY(EditAnywhere, Category = Settings)
		FVector2D DefaultIconSize;

	UPROPERTY() bool bInteractHidden;
	UPROPERTY() FSlateBrush IconBrush;
	UPROPERTY(Transient) TObjectPtr<UInteractionComponent> Component;
	
	void UpdateInteraction();
	virtual bool ShouldHide() const override;
	virtual void NativeConstruct() override;
};
