// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Engine/TargetPoint.h"
#include "TeleportPoint.generated.h"

UCLASS(meta = (ChildCanTick))
class TOROCORE_API ATeleportPoint : public ATargetPoint
{
	GENERATED_BODY()

public:

	ATeleportPoint();

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Actor)
		void TeleportActor(AActor* TargetActor) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Actor)
		void TeleportPlayer(const int32 PlayerIndex = 0) const;

#if WITH_EDITORONLY_DATA
private:
	UPROPERTY(EditAnywhere, Category = Tools)
		float DesiredOffset = 88.0f;
#endif
#if WITH_EDITOR
	UFUNCTION(CallInEditor, Category = Tools)
		void OffsetFromFloor();
	
	bool bDidOffset = false;
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};
