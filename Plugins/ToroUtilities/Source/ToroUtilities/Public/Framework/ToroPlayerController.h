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
	
	UFUNCTION(BlueprintCallable, Category = Input)
		virtual void SetInputModeData(const FGameInputModeData& InputMode);

	UFUNCTION(BlueprintPure, Category = Input)
		const FGameInputModeData& GetInputModeData() const { return InputModeData; }

protected:
	
	UPROPERTY() FGameInputModeData InputModeData;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	class UEnhancedInputLocalPlayerSubsystem* GetEnhancedInputSubsystem() const;
};
