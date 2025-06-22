// Copyright (C) RedCraft86. All Rights Reserved.

#include "Interaction/InteractionComponent.h"
#include "Framework/ToroPlayerController.h"
#include "Framework/ToroPlayerCharacter.h"
#include "Framework/ToroPlayerState.h"
#include "Helpers/ClassGetters.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.05f;
}

UInteractionComponent* UInteractionComponent::GetInteractor(const UObject* ContextObject, const int32 PlayerIndex)
{
	FIND_PLAYER_COMPONENT(UInteractionComponent)
}
