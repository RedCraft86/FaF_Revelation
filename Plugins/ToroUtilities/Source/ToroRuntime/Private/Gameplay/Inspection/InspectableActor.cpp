// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/Inspection/InspectableActor.h"

AInspectableActor::AInspectableActor()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AInspectableActor::OnBeginInteract_Implementation(AToroPlayerCharacter* Player, const FHitResult& Hit)
{
	if (HasValidArchive() && Manager)
	{
		Manager->BeginInspection(Archive);
	}
}

bool AInspectableActor::GetInteractInfo_Implementation(const FHitResult& Hit, FInteractionInfo& Info)
{
	return HasValidArchive() && Super::GetInteractInfo_Implementation(Hit, Info);
}

void AInspectableActor::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		Manager = UInspectionManager::Get(this);
		if (!HasValidArchive())
		{
			SetEnabled(false);
		}
	});
}

#if WITH_EDITOR
void AInspectableActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (Archive && !Archive->IsValidArchive())
	{
		Archive = nullptr;
	}
}
#endif
