// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroPlayerCharacter.h"
#include "Framework/ToroPlayerController.h"
#include "Components/PointLightComponent.h"
#include "Interaction/InteractionManager.h"
#include "Inspection/InspectionManager.h"
#include "Camera/CameraComponent.h"

namespace PlayerLockTags
{
	DEFINE_GAMEPLAY_TAG(PlayerLock)
	DEFINE_GAMEPLAY_TAG_CHILD(PlayerLock, MainMenu)
	DEFINE_GAMEPLAY_TAG_CHILD(PlayerLock, Startup)
	DEFINE_GAMEPLAY_TAG_CHILD(PlayerLock, Loading)
	DEFINE_GAMEPLAY_TAG_CHILD(PlayerLock, Dialogue)
	DEFINE_GAMEPLAY_TAG_CHILD(PlayerLock, Cinematic)
	DEFINE_GAMEPLAY_TAG_CHILD(PlayerLock, GameOver)
}

AToroPlayerCharacter::AToroPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	AutoReceiveInput = EAutoReceiveInput::Player0;

	PlayerLight = CreateDefaultSubobject<UPointLightComponent>("PlayerLight");
	PlayerLight->SetupAttachment(GetMesh());

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>("PlayerCamera");
	PlayerCamera->SetupAttachment(GetMesh());

	Inspection = CreateDefaultSubobject<UInspectionManager>("Inspection");
	Inspection->SetupAttachment(PlayerCamera);

	Interaction = CreateDefaultSubobject<UInteractionManager>("Interaction");
}
