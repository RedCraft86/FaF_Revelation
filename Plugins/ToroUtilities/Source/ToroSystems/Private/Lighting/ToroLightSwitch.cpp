// Copyright (C) RedCraft86. All Rights Reserved.

#include "Lighting/ToroLightSwitch.h"

AToroLightSwitch::AToroLightSwitch()
{
	PrimaryActorTick.TickGroup = TG_DuringPhysics;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>("CollisionBox");
	CollisionBox->SetupAttachment(GetRootComponent());
	CollisionBox->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	ActionManager = CreateDefaultSubobject<UWorldActionComponent>("ActionManager");
	ActionManager->bAutoConstruction = false;
}

void AToroLightSwitch::SetSwitchState(const bool bInState)
{
	if (bSwitchState != bInState)
	{
		bSwitchState = bInState;
		UpdateLights();

		ActionManager->SetActions(bSwitchState ? OnActions : OffActions, false);
		ActionManager->RunActions();
	}
}

void AToroLightSwitch::UpdateLights()
{
	ApplySwitchState(bSwitchState);
	for (AToroLightBase* Light : Lights)
	{
		if (Light) Light->SetEnabled(bSwitchState);
	}
}

void AToroLightSwitch::BeginPlay()
{
	Super::BeginPlay();
	UpdateLights();
}

void AToroLightSwitch::EnableStateChanged(const bool bIsEnabled)
{
	SetActorEnableCollision(bIsEnabled);
	SetActorTickEnabled(PrimaryActorTick.bStartWithTickEnabled && bIsEnabled);
}

FInteractionInfo AToroLightSwitch::GetInteractInfo_Implementation(const FHitResult& HitResult)
{
	return IsEnabled() ? Interaction : FInteractionInfo::GetEmpty();
}

void AToroLightSwitch::OnBeginInteract_Implementation(AToroPlayerCharacter* Player, const FHitResult& HitResult)
{
	SetSwitchState(!bSwitchState);
}

#if WITH_EDITOR
void AToroLightSwitch::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ActionManager->SetActions(OnActions, false);
	ActionManager->AppendActions(OffActions, true);
}
#endif
