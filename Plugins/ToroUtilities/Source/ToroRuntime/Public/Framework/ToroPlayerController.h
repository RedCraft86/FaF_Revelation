// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ExecPinEnums.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "ToroPlayerController.generated.h"

UENUM(BlueprintType)
enum class EGameInputMode : uint8
{
	UI_Only,
	GameOnly,
	GameAndUI
};

USTRUCT(BlueprintType)
struct TORORUNTIME_API FGameInputModeData
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

	UPROPERTY(EditAnywhere, Category = InputModeData, meta = (EditCondition = false, DisplayThumbnail = false))
		TObjectPtr<UUserWidget> FocusWidget;

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

	bool ShouldShowMouse() const { return InputMode != EGameInputMode::GameOnly && bShowMouseCursor; }
	
	const FInputModeDataBase& GetInputMode()
	{
		switch (InputMode)
		{
		case EGameInputMode::UI_Only:
			UI_Only.SetLockMouseToViewportBehavior(MouseLock);
			if (FocusWidget) UI_Only.SetWidgetToFocus(FocusWidget->TakeWidget());
			return UI_Only;
		
		case EGameInputMode::GameAndUI:
			GameAndUI.SetLockMouseToViewportBehavior(MouseLock);
			GameAndUI.SetHideCursorDuringCapture(bHideCursorOnCapture);
			if (FocusWidget) GameAndUI.SetWidgetToFocus(FocusWidget->TakeWidget());
			return GameAndUI;

		default: return GameOnly;
		}
	}
};

UCLASS()
class TORORUNTIME_API AToroPlayerController : public APlayerController
{
	GENERATED_BODY()

	friend class AToroSequenceActor;
	
public:

	AToroPlayerController();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

	UFUNCTION(BlueprintCallable, Category = Game, meta = (WorldContext = "WorldContextObject", DynamicOutputParam = "OutObject", DeterminesOutput = "Class", ExpandEnumAsExecs = "ReturnValue", AutoCreateRefTerm = "Class", CompactNodeTitle = "Get Player Controller"))
		static EToroValidPins GetToroPlayerController(AToroPlayerController*& OutObject, const UObject* WorldContextObject, const TSubclassOf<AToroPlayerController>& Class, const int32 PlayerIndex = 0);

	template <typename T = AToroPlayerController>
	static T* Get(const UObject* WorldContextObject, const int32 PlayerIndex = 0)
	{
		return Cast<T>(UGameplayStatics::GetPlayerController(WorldContextObject, PlayerIndex));
	}

	UFUNCTION(BlueprintCallable, Category = Input, meta = (AdvancedDisplay = "MouseLock, bHideCursorOnCapture, FocusWidget, PlayerIndex"))
		void SetGameInputMode(const EGameInputMode InputMode, const bool bMouseCursor = false, const EMouseLockMode MouseLock = EMouseLockMode::LockAlways, const bool bHideCursorOnCapture = true, UUserWidget* FocusWidget = nullptr);
	
	UFUNCTION(BlueprintCallable, Category = Input)
		void SetInputModeData(const FGameInputModeData& InputMode);

	UFUNCTION(BlueprintPure, Category = Input)
		const FGameInputModeData& GetInputModeData() const { return InputModeData; }

	UFUNCTION(BlueprintPure, Category = Input)
		const AToroSequenceActor* GetCinematicActor() const { return CinematicActor; }
	
protected:
	
	UPROPERTY() FGameInputModeData InputModeData;
	UPROPERTY() TObjectPtr<AToroSequenceActor> CinematicActor;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
