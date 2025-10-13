// Copyright (C) RedCraft86. All Rights Reserved.

#include "PlayerCharacter.h"
#include "Tutorials/TutorialManager.h"
#include "Inventory/InventoryManager.h"
#include "Inspection/InspectionManager.h"
#include "Interaction/InteractionManager.h"
#include "Narrative/NarrativeManager.h"

APlayerCharacter::APlayerCharacter()
	: ControlFlags(Player::DefaultAbilities), StateFlags(PSF_None)
{
	PrimaryActorTick.bCanEverTick = true;
}

void APlayerCharacter::OverrideControlFlags(const int32 InFlags)
{
	for (const EPlayerControlFlags Enum : TEnumRange<EPlayerControlFlags>())
	{
		InFlags & Enum ? SetControlFlag(Enum) : UnsetControlFlag(Enum);
	}
}

void APlayerCharacter::SetControlFlag(const EPlayerControlFlags InFlag)
{
	if (InFlag != PCF_None && !HasControlFlag(InFlag))
	{
		ControlFlags |= InFlag;
		switch (InFlag)
		{
			// TODO add more
			case PCF_CanInteract:	Interaction->SetEnabled(true); break;
			default: break;
		}
	}
}

void APlayerCharacter::UnsetControlFlag(const EPlayerControlFlags InFlag)
{
	if (InFlag != PCF_None && HasControlFlag(InFlag))
	{
		ControlFlags &= ~InFlag;
		switch (InFlag)
		{
			// TODO add more
			case PCF_CanInteract:	Interaction->SetEnabled(false); break;
			default: break;
		}
	}
}

bool APlayerCharacter::HasControlFlag(const EPlayerControlFlags InFlag) const
{
	return InFlag != PCF_None && ControlFlags & InFlag;
}

void APlayerCharacter::SetStateFlag(const EPlayerStateFlags InFlag)
{
	if (InFlag != PSF_None) StateFlags |= InFlag;
}

void APlayerCharacter::UnsetStateFlag(const EPlayerStateFlags InFlag)
{
	if (InFlag != PSF_None) StateFlags &= ~InFlag;
}

bool APlayerCharacter::HasStateFlag(const EPlayerStateFlags InFlag) const
{
	return InFlag != PSF_None && StateFlags & InFlag;
}

int32 APlayerCharacter::GetActivityFlags() const
{
	int32 Flags = PAF_None;
	if (HidingSpot.IsValid())
	{
		Flags |= PAF_Hiding;
	}
	if (OperatingActor.IsValid())
	{
		Flags |= PAF_Operating;
	}
	if (InspectionManager && InspectionManager->IsInspecting())
	{
		Flags |= PAF_Inspect;
	}
	if (InventoryManager && InventoryManager->IsInventoryOpen())
	{
		Flags |= PAF_Inventory;
	}
	if (TutorialManager && TutorialManager->IsShowingTutorial())
	{
		Flags |= PAF_Tutorial;
	}
	if (NarrativeManager && NarrativeManager->IsInDialogue())
	{
		Flags |= PAF_Dialogue;
	}
	return Flags;
}

void APlayerCharacter::SetHidingSpot(AActor* InSpot)
{
	HidingSpot = InSpot;
}

void APlayerCharacter::SetOperatingActor(AActor* InActor)
{
	OperatingActor = InActor;
}

bool APlayerCharacter::ShouldLockPlayer()
{
	return Super::ShouldLockPlayer() || GetActivityFlags() != PSF_None;
}

void APlayerCharacter::SlowTick()
{
	Super::SlowTick();
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimerForNextTick([this]()
	{
		TutorialManager = UTutorialManager::Get(this);
		InventoryManager = UInventoryManager::Get(this);
		InspectionManager = UInspectionManager::Get(this);
		NarrativeManager = UNarrativeManager::Get(this);
		Interaction->SetEnabled(HasControlFlag(PCF_CanInteract));
	});
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TickCameraLockOn(DeltaTime);
}

void APlayerCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
