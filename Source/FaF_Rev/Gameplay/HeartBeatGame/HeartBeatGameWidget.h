// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/RichTextBlockImageDecorator.h"
#include "HeartBeatGameWidget.generated.h"

class UImage;
class UButton;
class UTextBlock;

UCLASS(Abstract)
class FAF_REV_API UHeartBeatGameButton final : public UUserWidget
{
	GENERATED_BODY()

	friend class UHeartBeatGameWidget;

public:

	UPROPERTY(Transient, meta = (BindWidget))
		TObjectPtr<UButton> Button;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> SuccessAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> FailAnim;

protected:
	
	bool bAltKey = false;
	float Cooldown = 0.0f;
	bool bPressed = false;
	bool bStopTick = false;
	UPROPERTY(Transient) TObjectPtr<UHeartBeatGameWidget> Parent;
	
	UFUNCTION() void ButtonClicked();
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};

UCLASS(Abstract)
class FAF_REV_API UHeartBeatGameWidget final : public UUserWidget
{
	GENERATED_BODY()

	//friend class UHeartBeatGame;
	friend class UHeartBeatGameButton;

public:

	UPROPERTY(Transient, meta = (BindWidget))
		TObjectPtr<class UVirtualMouse> VirtualMouse;

	UPROPERTY(Transient, meta = (BindWidget))
		TObjectPtr<class UHorizontalBox> Container;

	UPROPERTY(Transient, meta = (BindWidget))
		TObjectPtr<UButton> BackingButton;

	UPROPERTY(Transient, meta = (BindWidget))
		TObjectPtr<UTextBlock> MaxChance;

	UPROPERTY(Transient, meta = (BindWidget))
		TObjectPtr<UTextBlock> CurChance;
	
	UPROPERTY(Transient, meta = (BindWidget))
		TObjectPtr<UImage> KeyImageA;

	UPROPERTY(Transient, meta = (BindWidget))
		TObjectPtr<UImage> KeyImageB;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> FadeAnim;
	
	UPROPERTY(Transient, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> ScrollAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> WrongAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> SwapAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		TSubclassOf<UHeartBeatGameButton> ButtonClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		float ScrollRate = 1.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		FSlateBrush EmptySpaceImage;
	
	UFUNCTION(BlueprintImplementableEvent)
		FRichImageRow GetKeyIcon(const FName& InKey);

	UFUNCTION(BlueprintCallable, Category = "HeartBeatGame")
		uint8 GetChances() const { return Chances; }

	UFUNCTION(BlueprintCallable, Category = "HeartBeatGame")
		FKey GetCurrentKey() const { return CurrentKey; }

	UFUNCTION(BlueprintCallable, Category = "HeartBeatGame")
		void StartGame(const FKey InKeyA, const FKey InKeyB, const uint8 InMaxChances, const TArray<FString> InSequence);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameCallback);
	UPROPERTY(BlueprintAssignable) FGameCallback OnSuccess;
	UPROPERTY(BlueprintAssignable) FGameCallback OnFail;

protected:
	
	uint8 Chances = 3;
	bool bInGame = false;
	TArray<FString> Sequence;
	TPair<FKey, FKey> KeyRange;
	bool bLastKey = false;
	FKey CurrentKey;
	
	//UPROPERTY(Transient) TObjectPtr<UHeartBeatGame> Controller;
	UPROPERTY(Transient) TArray<TObjectPtr<UHeartBeatGameButton>> Buttons;

	UFUNCTION() void OnWrongKey();
	UFUNCTION() void OnMissedKey();
	UFUNCTION() void OnCorrectKey();

	void AddChildEntries();
	void StartScrollAnim();

	void RemoveWidget();
	void ProcessNextButton();
	virtual void NativeConstruct() override;
};
