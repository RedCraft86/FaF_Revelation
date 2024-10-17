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

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> SuccessAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> FailAnim;

protected:
	
	bool bAltKey = false;
	bool bPressed = false;
	bool bStopTick = false;
	UPROPERTY(Transient) TObjectPtr<UHeartBeatGameWidget> Parent;
	
	void ButtonClicked(const bool bSuccess);
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};

UCLASS(Abstract)
class FAF_REV_API UHeartBeatGameWidget final : public UUserWidget
{
	GENERATED_BODY()

	friend class UHeartBeatGame;
	friend class UHeartBeatGameButton;

public:

	UPROPERTY(Transient, meta = (BindWidget))
		TObjectPtr<UTextBlock> TitleText;

	UPROPERTY(Transient, meta = (BindWidget))
		TObjectPtr<UWidget> VirtualCursor;

	UPROPERTY(Transient, meta = (BindWidget))
		TObjectPtr<class UHorizontalBox> Container;

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
		TObjectPtr<UWidgetAnimation> CorrectAnim;
	
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
		bool IsInGame() const { return bInGame; }
	
	UFUNCTION(BlueprintCallable, Category = "HeartBeatGame")
		uint8 GetChances() const { return Chances; }

	UFUNCTION(BlueprintCallable, Category = "HeartBeatGame")
		FKey GetCurrentKey() const { return CurrentKey; }

	UFUNCTION(BlueprintCallable, Category = "HeartBeatGame")
		void StartGame(const FKey InKeyA, const FKey InKeyB, const uint8 InMaxChances, const TArray<FString>& InSequence);

protected:
	
	uint8 Chances = 3;
	bool bInGame = false;
	TArray<FString> Sequence;
	TPair<FKey, FKey> KeyRange;
	bool bAltKey = false;
	FKey CurrentKey;
	
	UPROPERTY(Transient) TObjectPtr<UHeartBeatGame> Controller;
	UPROPERTY(Transient) TArray<TObjectPtr<UHeartBeatGameButton>> Buttons;

	UFUNCTION() void OnWrongKey();
	UFUNCTION() void OnMissedKey();
	UFUNCTION() void OnCorrectKey();

	void AddChildEntries();
	void StartScrollAnim();
	FVector2D GetCursorPos() const;

	void RemoveWidget();
	void ProcessNextButton();
	void PressKey(const FKey& InKey);
	void SetTitle(const FText& InTitle) const;
};
