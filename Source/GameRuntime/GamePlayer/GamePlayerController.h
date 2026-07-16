// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "Framework/ToroPlayerController.h"
#include "GamePlayerController.generated.h"

class UMessageManager;

UCLASS(Blueprintable, BlueprintType)
class AGamePlayerController final : public AToroPlayerController
{
	GENERATED_BODY()

public:

	AGamePlayerController();

	void SetGamePaused(const bool bPaused);

	UMessageManager* GetMessageManager() const { return MessageManager; }

private:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects, meta = (AllowPrivateAccess = true))
		TObjectPtr<UMessageManager> MessageManager;

	UPROPERTY(EditAnywhere, Category = Settings)
		TSubclassOf<class UGamePauseWidget> PauseWidgetClass;

	UPROPERTY(EditAnywhere, Category = Settings)
		TObjectPtr<class UInputMappingContext> InputMappings;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetCinematicMode(bool bInCinematicMode, bool bAffectsMovement, bool bAffectsTurning) override;
};
