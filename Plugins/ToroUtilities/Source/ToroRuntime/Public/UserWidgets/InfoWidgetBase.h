// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroWidgetBase.h"
#include "InfoWidgetBase.generated.h"

class UTextBlock;
class UToroUserSettings;

UCLASS(Abstract)
class TORORUNTIME_API UInfoWidgetBase : public UToroWidget
{
	GENERATED_BODY()

	friend class UToroSaveObject;
	
public:

	UInfoWidgetBase(const FObjectInitializer& ObjectInitializer);

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

	UPROPERTY() bool bShowFPS;
	UPROPERTY() float FPSTick;
	UPROPERTY() float TargetFPS;

	void UpdateFrameRate() const;
	void OnSettingsChanged(const UToroUserSettings* Settings);
	void OnSaveLoad(const UToroUserSettings* Settings);
	
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
