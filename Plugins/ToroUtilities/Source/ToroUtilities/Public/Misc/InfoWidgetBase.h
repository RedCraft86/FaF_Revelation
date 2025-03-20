// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroWidget.h"
#include "InfoWidgetBase.generated.h"

class UTextBlock;

UCLASS(Abstract)
class TOROUTILITIES_API UInfoWidgetBase : public UToroWidget
{
	GENERATED_BODY()

	friend class UToroUserSettings;
	friend class AToroPlayerController;
	
public:

	UInfoWidgetBase(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UPanelWidget> UnfocusedView;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> FrameRateText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> DeltaTimeText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> SaveAnim;

	UPROPERTY(EditAnywhere, Category = Settings)
		FLinearColor GoodFrameRateColor;

	UPROPERTY(EditAnywhere, Category = Settings)
		FLinearColor BadFrameRateColor;

protected:

	UPROPERTY() bool bPrePaused;
	UPROPERTY() bool bShowFPS;
	UPROPERTY() float FPSTick;
	UPROPERTY() float TargetFPS;

	void UpdateFrameRate() const;
	void OnFocusChanged(const bool bFocused) const;
	void OnSettingsChanged(const UToroUserSettings* Settings);
	void OnSaveLoad(const class UToroSaveObject* Object);

	virtual void InitWidget() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual bool ShouldBeHidden() override { return bWantHidden; }
};
