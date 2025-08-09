// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserWidgets/ToroWidgetBase.h"
#include "CutsceneSkipWidget.generated.h"

class UTextBlock;
class UProgressBar;
class AToroSequenceActor;

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

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> HoldAnim;

	UPROPERTY(EditAnywhere, Category = Settings)
		FKey SkipKey;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 0.5f, UIMin = 0.5f))
		float HoldTime;

	void ShowWidget(AToroSequenceActor* InSequence);
		
protected:

	UPROPERTY() bool bHolding;
	UPROPERTY() float Progress;
	UPROPERTY(Transient) TObjectPtr<AToroSequenceActor> Sequence;

	void SkipCutscene() const;
	void SetHoldingState(const bool InState);
	
	virtual void NativePreConstruct() override;
	virtual void InternalProcessDeactivation() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
};
