// Copyright (C) RedCraft86. All Rights Reserved.

#include "Framework/ToroPlayerCharacter.h"
#include "Framework/ToroPlayerController.h"
#include "Components/PointLightComponent.h"
#include "Interaction/InteractionManager.h"
#include "Libraries/ToroLightingUtils.h"
#include "Narrative/NarrativeManager.h"
#include "Camera/CameraComponent.h"
#include "ToroRuntime.h"

namespace PlayerLockTags
{
	DEFINE_GAMEPLAY_TAG(PlayerLock)
	DEFINE_GAMEPLAY_TAG_CHILD(PlayerLock, Loading)
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
	if (PlayerLockTags::IsValidTag(InTag))
	{
		LockTags.AddTag(InTag);
	}
}

void AToroPlayerCharacter::RemoveLockTag(const FGameplayTag InTag)
{
	if (PlayerLockTags::IsValidTag(InTag))
	{
		LockTags.RemoveTag(InTag);
	}
}

void AToroPlayerCharacter::ClearLockTags()
{
	LockTags.Reset();
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

bool AToroPlayerCharacter::ShouldLockPlayer()
{
	if (!LockTags.IsEmpty())
	{
		return true;
	}
	if (Narrative && Narrative->IsInDialogue())
	{
		return true;
	}
	if (UToroSettings::Get()->IsOnMap(this, EToroMapType::MainMenu))
	{
		return true;
	}
	if (const AToroPlayerController* PC = GetPlayerController(); PC && PC->bCinematicMode)
	{
		return true;
	}
	return false;
}

void AToroPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	Interaction->HandleTrace.BindUObject(this, &AToroPlayerCharacter::HandleInteraction);
	GetWorldTimerManager().SetTimerForNextTick([this]()
	{
		Narrative = UNarrativeManager::Get(this);
	});
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
