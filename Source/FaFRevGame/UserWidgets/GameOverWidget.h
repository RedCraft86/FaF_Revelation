// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroUtilities.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "UserWidgets/ExprTextBlock.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "GameOverWidget.generated.h"

USTRUCT(BlueprintType)
struct FAFREVGAME_API FGameOverInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = TutorialInfo)
		FText Name;

	UPROPERTY(EditAnywhere, Category = TutorialInfo)
		FText Description;

	UPROPERTY(EditAnywhere, Category = TutorialInfo)
		bool bRightText;

	FGameOverInfo(): bRightText(false) {}
};

UCLASS(Abstract)
class FAFREVGAME_API UGameOverWidget final : public UToroWidgetBase
{
	GENERATED_BODY()

public:

	UGameOverWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> NameText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UExprTextBlock> DescText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> RestartBtn;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> MainMenuBtn;

	UPROPERTY(EditAnywhere, Category = Settings)
		TMap<FGameplayTag, FGameOverInfo> GameOverEntries;

	UFUNCTION(BlueprintImplementableEvent)
		void OnShowTutorial(const bool bRightText);

	void ShowWidget(const FGameplayTag& InCharacter);

private:

	UPROPERTY(Transient) TObjectPtr<class AToroPlayerController> Controller;

	UFUNCTION() void Restart();
	UFUNCTION() void MainMenu();

	void FadeOut(const TFunction<void()>& Callback);
	
	virtual void InitWidget() override;
	virtual bool CanCreateWidget() const override { return UToroUtilSettings::Get()->IsOnGameplayMap(this); }
};
