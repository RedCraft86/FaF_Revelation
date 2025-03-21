// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ExecPinEnums.h"
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

	void ClearReferences();
	const FInputModeDataBase& GetInputMode();
	bool ShouldShowMouse() const { return InputMode != EGameInputMode::GameOnly && bShowMouseCursor; }
};

UCLASS()
class TOROUTILITIES_API AToroPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AToroPlayerController();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAnyKeyPressedEvent, const FKey&, Key);
	UPROPERTY(BlueprintAssignable) FAnyKeyPressedEvent OnAnyKeyPressed;

	UFUNCTION(BlueprintCallable, Category = Game, meta = (WorldContext = "ContextObject", DynamicOutputParam = "OutObject", DeterminesOutput = "Class", ExpandEnumAsExecs = "ReturnValue", AutoCreateRefTerm = "Class", CompactNodeTitle = "Get Player Controller"))
	static EToroValidPins GetToroPlayerController(AToroPlayerController*& OutObject, const UObject* ContextObject, const TSubclassOf<AToroPlayerController>& Class, const int32 PlayerIndex = 0);

	template <typename T = AToroPlayerController>
	static T* Get(const UObject* ContextObject, const int32 PlayerIndex = 0)
	{
		return Cast<T>(UGameplayStatics::GetPlayerController(ContextObject, PlayerIndex));
	}

	UFUNCTION(BlueprintCallable, Category = Input, meta = (AdvancedDisplay = "MouseLock, bHideCursorOnCapture, FocusWidget, PlayerIndex"))
		virtual void SetGameInputMode(const EGameInputMode InputMode, const bool bMouseCursor = false, const EMouseLockMode MouseLock = EMouseLockMode::LockAlways, const bool bHideCursorOnCapture = true, UUserWidget* FocusWidget = nullptr);

	UFUNCTION(BlueprintCallable, Category = Input)
		virtual void SetInputModeData(const FGameInputModeData& InputMode);

	UFUNCTION(BlueprintPure, Category = Input)
		const FGameInputModeData& GetInputModeData() const { return InputModeData; }

	UFUNCTION(BlueprintCallable, Category = Controller)
		void AddPauseRequest(const UObject* InObject);

	UFUNCTION(BlueprintCallable, Category = Controller)
		void RemovePauseRequest(const UObject* InObject);

	UFUNCTION(BlueprintCallable, Category = Controller)
		void SetGamePaused(const bool bInPaused);

	UFUNCTION(BlueprintCallable, Category = Controller)
		bool IsGamePaused() const { return bGamePaused; }
	
	UFUNCTION(BlueprintCallable, Category = Player)
		virtual void EnterCinematic(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = Player)
		virtual void ExitCinematic();
	
	UFUNCTION(BlueprintPure, Category = Player)
		AActor* GetCinematicActor() const { return CinematicActor; }

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnGamePaused, const bool)
	FOnGamePaused OnGamePaused;

protected:

	UPROPERTY() bool bGamePaused;
	UPROPERTY() FGameInputModeData InputModeData;
	UPROPERTY(Transient) TObjectPtr<AActor> CinematicActor;
	UPROPERTY(Transient) TSet<TWeakObjectPtr<const UObject>> PauseRequests;

	virtual void UpdatePauseState();
	virtual void OnAnyKeyEvent(FKey PressedKey);

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	class UEnhancedInputLocalPlayerSubsystem* GetEnhancedInputSubsystem() const;
};
