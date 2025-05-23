// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Helpers/ClassGetters.h"
#include "EnhancedInputSubsystems.h"
#include "DataTypes/InputModeData.h"
#include "GameFramework/PlayerController.h"
#include "ToroPlayerController.generated.h"

UCLASS()
class TOROUTILITIES_API AToroPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AToroPlayerController();

	PLAYER_CLASS_GETTER(AToroPlayerController, GetPlayerController)

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAnyKeyPressedEvent, const FKey&, Key);
	UPROPERTY(BlueprintAssignable) FAnyKeyPressedEvent OnAnyKeyPressed;
	
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
	virtual bool SetPause(bool bPause, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
	class UEnhancedInputLocalPlayerSubsystem* GetEnhancedInputSubsystem() const;
};
