// Copyright (C) RedCraft86. All Rights Reserved.

#include "WorldActions/Actions/ActorActions.h"
#include "Engine/StaticMeshActor.h"
#include "ToroVolume.h"
#include "ToroActor.h"

void FWTaskActorVisibility::RunEvent(const UObject* WorldContext)
{
	FOR_EACH_SOFT_PTR(Targets, AActor, {
		Ptr->SetActorHiddenInGame(ChangeType == ACTION_TYPE::Set ? !bVisible : !Ptr->IsHidden());
	})
}

void FWTaskActorCollision::RunEvent(const UObject* WorldContext)
{
	FOR_EACH_SOFT_PTR(Targets, AActor, {
		Ptr->SetActorEnableCollision(ChangeType == ACTION_TYPE::Set ? bCollision : !Ptr->GetActorEnableCollision());
	})
}

void FWTaskActorTags::OnBeginPlay(const UObject* WorldContext)
{
	Tags.Remove(NAME_None);
}

void FWTaskActorTags::RunEvent(const UObject* WorldContext)
{
	FOR_EACH_SOFT_PTR(Targets, AActor, {
		if (bRemoval)
		{
			Ptr->Tags.RemoveAll([this](const FName& Tag)
			{
				return Tags.Contains(Tag);
			});
		}
		else
		{
			for (const FName& Tag : Tags) Ptr->Tags.AddUnique(Tag);
		}
	})
}

void FWTaskActorEnabled::RunEvent(const UObject* WorldContext)
{
	FOR_EACH_SOFT_PTR(Targets, AActor, {
		if (AToroActor* ToroActor = Cast<AToroActor>(Ptr))
		{
			ToroActor->SetEnabled(ChangeType == ACTION_TYPE::Set
				? bEnabled : !ToroActor->IsEnabled());
		}
		else if (AToroVolume* ToroVolume = Cast<AToroVolume>(Ptr))
		{
			ToroVolume->SetEnabled(ChangeType == ACTION_TYPE::Set
				? bEnabled : !ToroVolume->IsEnabled());
		}
	})
}

void FWTaskSMMaterial::RunEvent(const UObject* WorldContext)
{
	FOR_EACH_SOFT_PTR(Targets, AStaticMeshActor, {
		if (UStaticMeshComponent* MeshComp = Ptr->GetStaticMeshComponent())
		{
			for (const TPair<uint8, TSoftObjectPtr<UMaterialInterface>>& Pair : Materials)
			{
				if (UMaterialInterface* Material = Pair.Value.LoadSynchronous())
				{
					MeshComp->SetMaterial(Pair.Key, Material);
				}
			}
		}
	})
}

void FWTaskSMPrimitiveData::RunEvent(const UObject* WorldContext)
{
	FOR_EACH_SOFT_PTR(Targets, AStaticMeshActor, {
		if (UStaticMeshComponent* MeshComp = Ptr->GetStaticMeshComponent())
		{
			for (const TPair<uint8, float>& Pair : Data)
			{
				bSerializable ? MeshComp->SetDefaultCustomPrimitiveDataFloat(Pair.Key, Pair.Value)
					: MeshComp->SetCustomPrimitiveDataFloat(Pair.Key, Pair.Value);
			}
		}
	})
}
