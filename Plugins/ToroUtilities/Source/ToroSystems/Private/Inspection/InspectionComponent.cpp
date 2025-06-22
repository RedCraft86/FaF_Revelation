// Copyright (C) RedCraft86. All Rights Reserved.

#include "Inspection/InspectionComponent.h"
#include "Framework/ToroPlayerController.h"
#include "Framework/ToroPlayerCharacter.h"
#include "Framework/ToroPlayerState.h"
#include "Helpers/ClassGetters.h"

UInspectionComponent::UInspectionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

UInspectionComponent* UInspectionComponent::GetInspector(const UObject* ContextObject, const int32 PlayerIndex)
{
	FIND_PLAYER_COMPONENT(UInspectionComponent)
}
