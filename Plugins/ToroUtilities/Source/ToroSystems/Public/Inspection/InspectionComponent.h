// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Components/SceneComponent.h"
#include "InspectionComponent.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class TOROSYSTEMS_API UInspectionComponent : public USceneComponent
{
	GENERATED_BODY()

public:

	UInspectionComponent();

	UFUNCTION(BlueprintPure, Category = Game, meta = (WorldContext = "ContextObject"))
		static UInspectionComponent* GetInspectionComponent(const UObject* ContextObject, const int32 PlayerIndex);

	static UInspectionComponent* Get(const UObject* ContextObject, const int32 PlayerIndex = 0)
	{
		return GetInspectionComponent(ContextObject, PlayerIndex);
	}
};
