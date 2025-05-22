// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Framework/ToroPlayerController.h"
#include "Framework/ToroGameInstance.h"
#include "Framework/ToroGameMode.h"

DEFINE_GAMEPLAY_TAG_CHILD(Character, Player)

AToroPlayerCharacter::AToroPlayerCharacter(): SlowTickInterval(0.1f)
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	AutoReceiveInput = EAutoReceiveInput::Player0;
	
	PlayerLight = CreateDefaultSubobject<UPointLightComponent>("PlayerLight");
	PlayerLight->SetupAttachment(GetCapsuleComponent());

	CharacterID = Tag_Character_Player;
	LightSettings.Intensity = 0.025f;
	LightSettings.AttenuationRadius = 500.0f;
	LightSettings.bUseTemperature = true;
	LightSettings.Temperature = 12000.0f;
	LightSettings.bLightCastShadows = false;
	LightSettings.bUseInverseSquaredFalloff = true;
	LightSettings.LightFalloffExponent = 1.0f;
	LightSettings.SpecularScale = 0.0f;
	ULightingDataLibrary::SetPointLightData(PlayerLight, LightSettings);
}

void AToroPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	GameMode = AToroGameMode::Get(this);
	GameInstance = UToroGameInstance::Get(this);
	PlayerController = GetController<AToroPlayerController>();
	GetWorldTimerManager().SetTimer(SlowTickTimer, this,
		&AToroPlayerCharacter::SlowTick, SlowTickInterval, true);
}

void AToroPlayerCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ULightingDataLibrary::SetPointLightData(PlayerLight, LightSettings);
}
