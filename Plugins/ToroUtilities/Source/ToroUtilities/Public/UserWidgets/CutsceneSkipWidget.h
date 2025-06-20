// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserWidgets/ToroWidgetBase.h"
#include "CutsceneSkipWidget.generated.h"

class UTextBlock;
class UProgressBar;

UCLASS(Abstract)
class TOROUTILITIES_API UCutsceneSkipWidget final : public UToroWidgetBase
{
	GENERATED_BODY()

public:

	UCutsceneSkipWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> SkipKeyText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UProgressBar> SkipProgressBar;

	UPROPERTY(EditAnywhere, Category = Settings)
		FKey SkipKey;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 0.5f, UIMin = 0.5f))
		float HoldTime;

protected:

	UPROPERTY() bool bHolding;
	UPROPERTY() float Progress;
	UPROPERTY(Transient) TObjectPtr<class AToroSequenceActor> Sequence;

	void SkipCutscene();
	virtual void NativePreConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
};
