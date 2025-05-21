// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Engine/TargetPoint.h"
#include "TeleportPoint.generated.h"

UCLASS()
class TOROCORE_API ATeleportPoint : public ATargetPoint
{
	GENERATED_BODY()

public:

	ATeleportPoint();

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Actor)
		void TeleportActor(AActor* TargetActor) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Actor)
		void TeleportPlayer() const;

private:

	virtual void BeginPlay() override;
#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};
