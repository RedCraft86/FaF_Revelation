// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroActor.h"
#include "WorldEventComponent.h"
#include "WorldEventActor.generated.h"

UCLASS(meta = (HiddenCategories = "Collision, Actor"))
class TOROGAMEPLAY_API AWorldEventActor final : public AToroActor
{
	GENERATED_BODY()

public:

	AWorldEventActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Subobjects")
		TObjectPtr<UWorldEventComponent> WorldEvents;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "WorldEvents")
		void RunEvents() const;
};
