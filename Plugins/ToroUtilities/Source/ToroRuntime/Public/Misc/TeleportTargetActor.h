// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Engine/TargetPoint.h"
#include "Characters/ToroPlayerBase.h"
#include "TeleportTargetActor.generated.h"

UCLASS(Blueprintable, meta = (HiddenCategories = "Rendering"))
class TORORUNTIME_API ATeleportTargetActor final : public ATargetPoint
{
	GENERATED_BODY()

public:

	ATeleportTargetActor();

	UPROPERTY(EditAnywhere, Category = Settings)
		TSoftObjectPtr<AToroPlayerBase> PlayerRef;

	UFUNCTION(BlueprintCallable, Category = Actor)
		void TeleportActor(AActor* TargetActor);
	
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Actor)
		void TeleportPlayer() const;

private:
	
	virtual void BeginPlay() override;
#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};
