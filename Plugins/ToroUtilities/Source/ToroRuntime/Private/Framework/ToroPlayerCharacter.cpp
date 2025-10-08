// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroPlayerCharacter.h"
#include "Framework/ToroPlayerController.h"
#include "Components/PointLightComponent.h"
#include "Interaction/InteractionManager.h"
#include "Libraries/ToroLightingUtils.h"
#include "Camera/CameraComponent.h"
#include "ToroRuntime.h"

namespace PlayerLockTags
{
	DEFINE_GAMEPLAY_TAG(PlayerLock)
	DEFINE_GAMEPLAY_TAG_CHILD(PlayerLock, MainMenu)
	DEFINE_GAMEPLAY_TAG_CHILD(PlayerLock, Loading)
	DEFINE_GAMEPLAY_TAG_CHILD(PlayerLock, Dialogue)
	DEFINE_GAMEPLAY_TAG_CHILD(PlayerLock, Cinematic)
}

AToroPlayerCharacter::AToroPlayerCharacter()
	: SlowTickInterval(0.1f), LockTags({PlayerLockTags::TAG_Loading.GetTag()})
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	AutoReceiveInput = EAutoReceiveInput::Player0;

	CharacterID = CharacterTags::TAG_Player;

	PlayerLight = CreateDefaultSubobject<UPointLightComponent>("PlayerLight");
	PlayerLight->SetupAttachment(GetMesh());

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>("PlayerCamera");
	PlayerCamera->SetupAttachment(GetMesh());

	Interaction = CreateDefaultSubobject<UInteractionManager>("Interaction");

	CharacterID = CharacterTags::TAG_Player;
	LightSettings.Intensity = 0.025f;
	LightSettings.AttenuationRadius = 500.0f;
	LightSettings.bUseTemperature = true;
	LightSettings.Temperature = 12000.0f;
	LightSettings.bLightCastShadows = false;
	LightSettings.bUseInverseSquaredFalloff = true;
	LightSettings.LightFalloffExponent = 1.0f;
	LightSettings.SpecularScale = 0.0f;
	UToroLightingUtils::SetPointLightProperties(PlayerLight, LightSettings);
}

void AToroPlayerCharacter::AddLockTag(const FGameplayTag InTag)
{
	if (PlayerLockTags::IsValidTag(InTag) && !LockTags.HasTagExact(InTag))
	{
		LockTags.AddTagFast(InTag);
		OnLockTagsChanged();
	}
}

void AToroPlayerCharacter::ClearLockTag(const FGameplayTag InTag)
{
	if (PlayerLockTags::IsValidTag(InTag) && LockTags.RemoveTag(InTag))
	{
		OnLockTagsChanged();
	}
}

bool AToroPlayerCharacter::HasLockTag(const FGameplayTag InTag) const
{
	return PlayerLockTags::IsValidTag(InTag) && LockTags.HasTag(InTag);
}

void AToroPlayerCharacter::ExitCinematic()
{
	if (AToroPlayerController* PC = GetPlayerController())
	{
		PC->ExitCinematic();
		ClearLockTag(PlayerLockTags::TAG_Cinematic.GetTag());
	}
}

void AToroPlayerCharacter::EnterCinematic(AActor* InInstigator)
{
	if (AToroPlayerController* PC = GetPlayerController())
	{
		PC->EnterCinematic(InInstigator);
		AddLockTag(PlayerLockTags::TAG_Cinematic.GetTag());
	}
}

void AToroPlayerCharacter::ClearLockOnTarget()
{
	LockOnTarget.Reset();
}

void AToroPlayerCharacter::SetLockOnTarget(AActor* InTarget)
{
	if (!InTarget)
	{
		ClearLockOnTarget();
	}

	LockOnTarget = InTarget;
}

void AToroPlayerCharacter::SetLightSettings(const FPointLightProperties& InSettings)
{
	LightSettings = InSettings;
	UToroLightingUtils::SetPointLightProperties(PlayerLight, LightSettings);
}

bool AToroPlayerCharacter::GetViewTarget_Implementation(FVector& Location) const
{
	if (LockOnTarget.IsValid())
	{
		Location = ICharInterface::GetFocusPoint(LockOnTarget.Get());
		return true;
	}
	return false;
}

void AToroPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	Interaction->HandleTrace.BindUObject(this, &AToroPlayerCharacter::HandleInteraction);
	if (UToroSettings::Get()->IsOnMap(this, EToroMapType::MainMenu))
	{
		LockTags.AddTag(PlayerLockTags::TAG_MainMenu);
	}
}

void AToroPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (SlowTickTime >= SlowTickInterval)
	{
		SlowTick();
		SlowTickTime = 0.0f;
	}
	else SlowTickTime += DeltaTime;
}

void AToroPlayerCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	LockTags.RemoveTag(PlayerLockTags::TAG_PlayerLock);
	UToroLightingUtils::SetPointLightProperties(PlayerLight, LightSettings);
}
