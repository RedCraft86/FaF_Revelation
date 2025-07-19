// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroUtilities.h"
#include "Components/Button.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "GameOverWidget.generated.h"

UCLASS(Abstract)
class FAFREVGAME_API UGameOverWidget final : public UToroWidgetBase
{
	GENERATED_BODY()

public:

	UGameOverWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> RestartBtn;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> MainMenuBtn;

private:

	UPROPERTY(Transient) TObjectPtr<class AToroPlayerController> Controller;

	UFUNCTION() void Restart();
	UFUNCTION() void MainMenu();
	void FadeOut(const TFunction<void()>& Callback);
	
	virtual void InitWidget() override;
	virtual bool CanCreateWidget() const override { return UToroUtilSettings::Get()->IsOnGameplayMap(this); }
};
