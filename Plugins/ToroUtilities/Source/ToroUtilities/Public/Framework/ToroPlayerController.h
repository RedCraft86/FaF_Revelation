// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "EnhancedInputSubsystems.h"
#include "Helpers/ClassGetters.h"
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

protected:

	virtual void BeginPlay() override;
	class UEnhancedInputLocalPlayerSubsystem* GetEnhancedInputSubsystem() const;
};
