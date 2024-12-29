// Copyright (C) RedCraft86. All Rights Reserved.

#include "WorldEvents/Events/ActorEvents.h"
#include "Engine/StaticMeshActor.h"
#include "ToroVolume.h"
#include "ToroActor.h"

void FWEActorVisibility::RunEvent(const UObject* WorldContext)
{
	for (const TSoftObjectPtr<AActor>& Actor : Targets)
	{
		if (AActor* ActorPtr = Actor.LoadSynchronous())
		{
			ActorPtr->SetActorHiddenInGame(ChangeType == EWorldEventChangeType::Set
				? !bVisible : !ActorPtr->IsHidden());
		}
	}
}

void FWEActorCollision::RunEvent(const UObject* WorldContext)
{
	for (const TSoftObjectPtr<AActor>& Actor : Targets)
	{
		if (AActor* ActorPtr = Actor.LoadSynchronous())
		{
			ActorPtr->SetActorHiddenInGame(ChangeType == EWorldEventChangeType::Set
				? bCollision : !ActorPtr->GetActorEnableCollision());
		}
	}
}

void FWEActorTags::OnBeginPlay(const UObject* WorldContext)
{
	Tags.Remove(NAME_None);
}

void FWEActorTags::RunEvent(const UObject* WorldContext)
{
	for (const TSoftObjectPtr<AActor>& Actor : Targets)
	{
		if (AActor* ActorPtr = Actor.LoadSynchronous())
		{
			if (bRemoval)
			{
				ActorPtr->Tags.RemoveAll([this](const FName& Tag)
				{
					return Tags.Contains(Tag);
				});
			}
			else
			{
				for (const FName& Tag : Tags) ActorPtr->Tags.AddUnique(Tag);
			}
		}
	}
}

void FWEActorEnabled::RunEvent(const UObject* WorldContext)
{
	for (const TSoftObjectPtr<AActor>& Actor : Targets)
	{
		if (AActor* ActorPtr = Actor.LoadSynchronous())
		{
			if (AToroActor* ToroActor = Cast<AToroActor>(ActorPtr))
			{
				ToroActor->SetEnabled(ChangeType == EWorldEventChangeType::Set
					? bEnabled : !ToroActor->IsEnabled());
			}
			else if (AToroVolume* ToroVolume = Cast<AToroVolume>(ActorPtr))
			{
				ToroVolume->SetEnabled(ChangeType == EWorldEventChangeType::Set
					? bEnabled : !ToroVolume->IsEnabled());
			}
		}
	}
}

void FWEStaticMeshMaterial::RunEvent(const UObject* WorldContext)
{
	for (const TSoftObjectPtr<AStaticMeshActor>& Actor : Targets)
	{
		const AStaticMeshActor* ActorPtr = Actor.LoadSynchronous();
		if (UStaticMeshComponent* MeshComp = ActorPtr ? ActorPtr->GetStaticMeshComponent() : nullptr)
		{
			for (const TPair<uint8, TSoftObjectPtr<UMaterialInterface>>& Pair : Materials)
			{
				if (UMaterialInterface* Material = Pair.Value.LoadSynchronous())
				{
					MeshComp->SetMaterial(Pair.Key, Material);
				}
			}
		}
	}
}

void FWEStaticMeshPrimitiveData::RunEvent(const UObject* WorldContext)
{
	for (const TSoftObjectPtr<AStaticMeshActor>& Actor : Targets)
	{
		const AStaticMeshActor* ActorPtr = Actor.LoadSynchronous();
		if (UStaticMeshComponent* MeshComp = ActorPtr ? ActorPtr->GetStaticMeshComponent() : nullptr)
		{
			for (const TPair<uint8, float>& Pair : Data)
			{
				MeshComp->SetCustomPrimitiveDataFloat(Pair.Key, Pair.Value);
			}
		}
	}
}
