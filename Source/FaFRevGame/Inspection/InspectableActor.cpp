// Copyright (C) RedCraft86. All Rights Reserved.

#include "InspectableActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/GamePlayer.h"

AInspectableActor::AInspectableActor(): TurnSpeed(10.0f), InspectScale(1.0f)
{
	PrimaryActorTick.TickGroup = TG_DuringPhysics;

	InspectRoot = CreateDefaultSubobject<USceneComponent>("InspectRoot");
	InspectRoot->SetupAttachment(GetRootComponent());
}

void AInspectableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!CurrentTransform.Equals(TargetTransform))
	{
		CurrentTransform = UKismetMathLibrary::TInterpTo(CurrentTransform,
			TargetTransform, DeltaTime, 10.0f);
		InspectRoot->SetWorldTransform(CurrentTransform);
	}
	if (CachedPlayer)
	{
		TargetTransform = CachedPlayer->InspectRoot->GetComponentTransform();
	}
}

void AInspectableActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	InspectRoot->SetRelativeTransform(FTransform::Identity);
	CurrentTransform = FTransform::Identity;
}

void AInspectableActor::Exit_Implementation()
{
	if (CachedPlayer)
	{
		CachedPlayer->SetInspectable(nullptr, 0.0f);
		CachedPlayer->GetPlayerController()->RemovePauseRequest(this);
	}
	CachedPlayer = nullptr;
	TargetTransform = SceneRoot->GetComponentTransform();
}

FInteractionInfo AInspectableActor::GetInteractionInfo_Implementation(const FHitResult& HitResult)
{
	return Interaction;
}

void AInspectableActor::OnBeginInteract_Implementation(AGamePlayer* Player, const FHitResult& HitResult)
{
	Player->Inventory->AddArchive(ArchiveEntry);
	Player->SetInspectable(this, TurnSpeed);
	Player->GetPlayerController()->AddPauseRequest(this);
	TargetTransform = Player->InspectRoot->GetComponentTransform();
	TargetTransform.SetScale3D(InspectScale);
	CachedPlayer = Player;
}
