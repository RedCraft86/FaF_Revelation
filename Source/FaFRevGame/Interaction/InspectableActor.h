// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Actors/ToroActor.h"
#include "Player/GamePlayerChar.h"
#include "InspectableActor.generated.h"

UCLASS()
class FAFREVGAME_API AInspectableActor final : public AToroActor, public IExitInterface
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

	UFUNCTION(BlueprintCallable, Category = Inspectable)
		void BeginInspection(AGamePlayerChar* Player);

private:

	FTimerHandle LagTimer;
	FToroInterpFloat ScaleLerp;
	TObjectPtr<AGamePlayerChar> PlayerChar;

	void HandleRemoveLag();
	virtual void Exit_Implementation() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnConstruction(const FTransform& Transform) override;
};
