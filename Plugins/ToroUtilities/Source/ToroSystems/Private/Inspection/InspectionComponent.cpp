// Copyright (C) RedCraft86. All Rights Reserved.

#include "Inspection/InspectionComponent.h"
#include "Framework/ToroPlayerCharacter.h"
#include "Framework/ToroWidgetManager.h"
#include "Framework/ToroPlayerState.h"

UInspectionComponent::UInspectionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

UInspectionComponent* UInspectionComponent::GetInspectionComponent(const UObject* ContextObject, const int32 PlayerIndex)
{
	FIND_PLAYER_COMPONENT(UInspectionComponent)
}
