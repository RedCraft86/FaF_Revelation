// Copyright (C) RedCraft86. All Rights Reserved.

#include "DoorLink.h"
#include "DoorBase.h"

ADoorLink::ADoorLink()
{
	PrimaryActorTick.bCanEverTick = true;
	bSmartLinkIsRelevant = true;
}

#if WITH_EDITORONLY_DATA
void ADoorLink::AutoTarget()
{
	if (LinkedDoor)
	{
		FVector Origin, Extent;
		LinkedDoor->GetActorBounds(true, Origin, Extent, false);
		Origin.Z = GetActorLocation().Z;

		bool bDirection = Extent.X > Extent.Y; 
		if (bSwapXY) bDirection = !bDirection;

		const FVector WorldForward = bDirection ? FVector::XAxisVector : FVector::YAxisVector;
		const FVector LocalForward = GetTransform().InverseTransformVectorNoScale(WorldForward);
		const FVector LocalOffset = GetTransform().InverseTransformPosition(Origin);
		const float Distance = FMath::Max3(Extent.X, Extent.Y, 5.0) * OffsetFactor;
		
		if (PointLinks.IsEmpty())
		{
			PointLinks = {FNavigationLink()};
		}
		else if (PointLinks.Num() > 1)
		{
			PointLinks.SetNum(1);
		}

		PointLinks[0].Left = LocalOffset + (LocalForward * Distance);
		PointLinks[0].Right = LocalOffset + (LocalForward * -Distance);
		CopyEndPointsFromSimpleLinkToSmartLink();

		MarkPackageDirty();
	}
}
#endif

void ADoorLink::OnPawnReached(AActor* Pawn, const FVector& Dest)
{
	if (!LinkedDoor)
	{
		SetSmartLinkEnabled(false);
		return;
	}
	if (Pawn)
	{
		if (LinkedDoor->IsLocked())
		{
			ICharInterface::OnPathingRejected(Pawn, EPathingRejectType::Door);
		}
		else
		{
			IInteractionInterface::PawnInteract(LinkedDoor, Pawn);
			ICharInterface::OnEntityInteraction(Pawn, ECharInteractType::Door, LinkedDoor);
		}

		ResumePathFollowing(Pawn);
	}
}

void ADoorLink::BeginPlay()
{
	Super::BeginPlay();
	SetSmartLinkEnabled(true);
	OnSmartLinkReached.AddDynamic(this, &ADoorLink::OnPawnReached);
}

void ADoorLink::PostInitializeComponents()
{
	// Fix issue where if this array is not empty, the link is treated
	// as a smart link AND a simple link which the AI randomly chose from
	PointLinks.Empty();

	Super::PostInitializeComponents();
}
