// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Engine/TargetPoint.h"
#include "TeleportTargetActor.generated.h"

UCLASS()
class TOROCORE_API ATeleportTargetActor : public ATargetPoint
{
	GENERATED_BODY()

public:

	ATeleportTargetActor();

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
