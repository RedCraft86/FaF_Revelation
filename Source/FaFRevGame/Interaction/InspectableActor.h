// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Actors/ToroActor.h"
#include "InteractionInterface.h"
#include "Player/GamePlayerChar.h"
#include "InspectableActor.generated.h"

UCLASS()
class FAFREVGAME_API AInspectableActor final : public AToroActor, public IExitInterface, public IInteractionInterface
{
	GENERATED_BODY()

public:

	AInspectableActor();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USpringArmComponent> InspectRoot;

	UPROPERTY(EditAnywhere, Category = Settings)
		FVector2D TurningSpeed;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 0.1f, UIMin = 0.1f, AllowPreserveRatio = true))
		FVector InspectScale;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float ScaleSpeed;

	UPROPERTY(EditAnywhere, Category = Settings, AdvancedDisplay)
		FInteractionInfo InteractionInfo;

	virtual void Exit_Implementation() override;
	virtual void OnBeginInteract_Implementation(AGamePlayerChar* Player, const FHitResult& HitResult) override;
	virtual FInteractionInfo GetInteractionInfo_Implementation(const FHitResult& HitResult) override
	{
		InteractionInfo.bEnabled = InteractionInfo.bEnabled && IsEnabled();
		return InteractionInfo;
	}

private:

	FTimerHandle LagTimer;
	FToroInterpFloat ScaleLerp;
	TObjectPtr<AGamePlayerChar> PlayerChar;

	void HandleRemoveLag();
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnConstruction(const FTransform& Transform) override;
};
