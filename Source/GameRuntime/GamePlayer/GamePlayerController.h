// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "Framework/ToroPlayerController.h"
#include "GamePlayerController.generated.h"

UCLASS(Blueprintable, BlueprintType)
class AGamePlayerController final : public AToroPlayerController
{
	GENERATED_BODY()

public:

	AGamePlayerController();

	void SetGamePaused(const bool bPaused);

private:

	UPROPERTY(EditAnywhere, Category = Settings)
		TSubclassOf<class UGamePauseWidget> PauseWidgetClass;

	UPROPERTY(EditAnywhere, Category = Settings)
		TSubclassOf<class UGameplayWidget> GameplayWidgetClass;

	UPROPERTY(EditAnywhere, Category = Settings)
		TObjectPtr<class UInputMappingContext> InputMappings;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetCinematicMode(bool bInCinematicMode, bool bAffectsMovement, bool bAffectsTurning) override;
};
