// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/RichTextBlockImageDecorator.h"
#include "KeyPressGameWidget.generated.h"

UCLASS(Abstract)
class FAF_REV_API UKeyPressGameButton final : public UUserWidget
{
	GENERATED_BODY()

	friend class UKeyPressGameWidget;

public:

	UPROPERTY(Transient, meta = (BindWidget))
		TObjectPtr<class UImage> KeyImage;
	
	UPROPERTY(Transient, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> ShowAnim;
	
	UPROPERTY(Transient, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> SuccessAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> FailAnim;

	UFUNCTION(BlueprintImplementableEvent)
		FRichImageRow GetKeyIcon(const FName& InKey);

protected:

	FKey Key;

	void WrongAnim();
	void RemoveWidget();
	void Setup(const FKey& InKey);
};

UCLASS(Abstract)
class FAF_REV_API UKeyPressGameWidget final : public UUserWidget
{
	GENERATED_BODY()

	friend class UKeyPressGame;

public:

	UPROPERTY(Transient, meta = (BindWidget))
		TObjectPtr<class UProgressBar> Progress;

	UPROPERTY(Transient, meta = (BindWidget))
		TObjectPtr<class UHorizontalBox> ButtonContainer;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		TSubclassOf<UKeyPressGameButton> ButtonClass;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> FadeAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> FailAnim;

protected:

	UPROPERTY(Transient)
		TArray<TObjectPtr<UKeyPressGameButton>> Buttons;
	
	void WrongKey();
	void RemoveKey();
	void AddKey(const FKey& InKey);
	void RemoveWidget();

	virtual void NativeConstruct() override;
};
