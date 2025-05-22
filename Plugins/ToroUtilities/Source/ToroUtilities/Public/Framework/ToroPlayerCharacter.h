// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Helpers/ClassGetters.h"
#include "Actors/ToroCharacter.h"
#include "DataTypes/LightingData.h"
#include "Components/PointLightComponent.h"
#include "ToroPlayerCharacter.generated.h"

DECLARE_GAMEPLAY_TAG_CHILD(Character, Player)

UCLASS()
class TOROUTILITIES_API AToroPlayerCharacter : public AToroCharacter
{
	GENERATED_BODY()

public:

	AToroPlayerCharacter();

	PLAYER_CLASS_GETTER(AToroPlayerCharacter, GetPlayerPawn);

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UPointLightComponent> PlayerLight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Tick, meta = (ClampMin = 0.05f, UIMin = 0.05f))
		float SlowTickInterval;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, AdvancedDisplay)
		FPointLightData LightSettings;

protected:

	UPROPERTY() FTimerHandle SlowTickTimer;
	UPROPERTY(Transient) TObjectPtr<class AToroGameMode> GameMode;
	UPROPERTY(Transient) TObjectPtr<class UToroGameInstance> GameInstance;
	UPROPERTY(Transient) TObjectPtr<class AToroPlayerController> PlayerController;

	virtual void SlowTick() {}
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
};
