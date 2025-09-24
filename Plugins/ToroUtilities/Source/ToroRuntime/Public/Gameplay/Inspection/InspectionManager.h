// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Components/SceneComponent.h"
#include "Helpers/ClassGetterMacros.h"
#include "Framework/ToroPlayerCharacter.h"
#include "InspectionManager.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (Player), meta = (BlueprintSpawnableComponent))
class TORORUNTIME_API UInspectionManager : public USceneComponent
{
	GENERATED_BODY()

public:

	UInspectionManager() {}
	
	PLAYER_COMPONENT_GETTER(UInspectionManager, AToroPlayerCharacter, Inspection)

	UFUNCTION(BlueprintCallable, Category = Statics, meta = (WorldContext = "ContextObject"))
	static UInspectionManager* GetInspectionManager(const UObject* ContextObject, const int32 PlayerIdx = 0)
	{
		return Get(ContextObject, PlayerIdx);
	}

	// TODO
};
