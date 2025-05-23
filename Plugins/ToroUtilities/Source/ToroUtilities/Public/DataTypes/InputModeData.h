// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "InputModeData.generated.h"

UENUM(BlueprintType)
enum class EGameInputMode : uint8
{
	UI_Only,
	GameOnly,
	GameAndUI
};

USTRUCT(BlueprintType)
struct TOROUTILITIES_API FGameInputModeData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = InputModeData)
		EGameInputMode InputMode;

	UPROPERTY(EditAnywhere, Category = InputModeData, meta = (EditCondition = "InputMode != EGameInputMode::GameOnly", EditConditionHides))
		bool bShowMouseCursor;

	UPROPERTY(EditAnywhere, Category = InputModeData, meta = (EditCondition = "InputMode != EGameInputMode::GameOnly", EditConditionHides))
		EMouseLockMode MouseLock;
	
	UPROPERTY(EditAnywhere, Category = InputModeData, meta = (EditCondition = "InputMode == EGameInputMode::GameAndUI", EditConditionHides))
		bool bHideCursorOnCapture;

	UPROPERTY(VisibleAnywhere, Category = InputModeData, meta = (DisplayThumbnail = false))
		TWeakObjectPtr<UUserWidget> FocusWidget;

private:
	
	FInputModeUIOnly UI_Only;
	FInputModeGameOnly GameOnly;
	FInputModeGameAndUI GameAndUI;

public:
	
	FGameInputModeData()
		: InputMode(EGameInputMode::GameOnly)
		, bShowMouseCursor(false)
		, MouseLock(EMouseLockMode::LockAlways)
		, bHideCursorOnCapture(true)
		, FocusWidget(nullptr)
	{}

	FGameInputModeData(const EGameInputMode InInputMode, const bool bInShowMouseCursor, const EMouseLockMode InMouseLock = EMouseLockMode::LockAlways, const bool bInHideCursorOnCapture = true, UUserWidget* InFocusWidget = nullptr)
		: InputMode(InInputMode)
		, bShowMouseCursor(bInShowMouseCursor)
		, MouseLock(InMouseLock)
		, bHideCursorOnCapture(bInHideCursorOnCapture)
		, FocusWidget(InFocusWidget)
	{}

	void ClearReferences()
	{
		UI_Only = FInputModeUIOnly();
		GameAndUI = FInputModeGameAndUI();
		FocusWidget = nullptr;
	}
	
	const FInputModeDataBase& GetInputMode()
	{
		switch (InputMode)
		{
		case EGameInputMode::UI_Only:
			UI_Only.SetLockMouseToViewportBehavior(MouseLock);
			if (FocusWidget.IsValid()) UI_Only.SetWidgetToFocus(FocusWidget->TakeWidget());
			return UI_Only;
		
		case EGameInputMode::GameAndUI:
			GameAndUI.SetLockMouseToViewportBehavior(MouseLock);
			GameAndUI.SetHideCursorDuringCapture(bHideCursorOnCapture);
			if (FocusWidget.IsValid()) GameAndUI.SetWidgetToFocus(FocusWidget->TakeWidget());
			return GameAndUI;

		default: return GameOnly;
		}
	}
	
	bool ShouldShowMouse() const { return InputMode != EGameInputMode::GameOnly && bShowMouseCursor; }
};