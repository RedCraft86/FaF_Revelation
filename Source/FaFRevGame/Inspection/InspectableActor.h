// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ExitInterface.h"
#include "Actors/ToroActor.h"
#include "Interaction/InteractionInterface.h"
#include "InspectableActor.generated.h"

UCLASS()
class FAFREVGAME_API AInspectableActor : public AToroActor, public IInteractionInterface, public IExitInterface
{
	GENERATED_BODY()

public:

	AInspectableActor();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> InspectRoot;

	UPROPERTY(EditAnywhere, Category = Settings)
		float TurnSpeed;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (AllowPreserveRatio = true))
		FVector InspectScale;

	UPROPERTY(EditAnywhere, Category = Settings, AdvancedDisplay)
		FInteractionInfo Interaction;

private:

	UPROPERTY() FTransform TargetTransform;
	UPROPERTY() FTransform CurrentTransform;
	UPROPERTY(Transient) TObjectPtr<AGamePlayer> CachedPlayer;

	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Exit_Implementation() override;
	virtual FInteractionInfo GetInteractionInfo_Implementation(const FHitResult& HitResult) override;
	virtual void OnBeginInteract_Implementation(AGamePlayer* Player, const FHitResult& HitResult) override;
};
