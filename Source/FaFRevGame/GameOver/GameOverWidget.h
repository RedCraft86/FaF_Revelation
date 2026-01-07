// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroRuntime.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "UserInterface/ExprTextBlock.h"
#include "UserInterface/ToroManagedWidget.h"
#include "GameOverWidget.generated.h"

UCLASS(Abstract)
class FAFREVGAME_API UGameOverWidget final : public UToroManagedWidget
{
	GENERATED_BODY()

public:

	UGameOverWidget(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintCallable, Category = Gameplay, meta = (WorldContext = "ContextObject"))
		static void ShowGameOver(const UObject* ContextObject);

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> LabelText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UExprTextBlock> ContentText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> RetryButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> HintButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> ShowAnim;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> HintAnim;

	UFUNCTION() void OnRetry();
	UFUNCTION() void OnHint() { PlayAnimation(HintAnim); }

	virtual void PushWidget() override;
	virtual void InitWidget(APlayerController* Controller) override;
	virtual bool CanCreateWidget(const UObject* ContextObject) const override
	{
		return UToroSettings::Get()->IsOnMap(ContextObject, EToroMapType::Gameplay);
	}
};
