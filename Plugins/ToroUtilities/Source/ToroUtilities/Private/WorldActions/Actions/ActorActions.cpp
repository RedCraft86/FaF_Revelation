// Copyright (C) RedCraft86. All Rights Reserved.

#include "WorldActions/Actions/ActorActions.h"
#include "Engine/StaticMeshActor.h"

void FWAActorVisibility::RunAction()
{
	FOR_EACH_SOFT_PTR(Targets, AActor, {
		Ptr->SetActorHiddenInGame(bToggle ? !Ptr->IsHidden() : !bVisible);
	})
}

void FWAActorCollision::RunAction()
{
	FOR_EACH_SOFT_PTR(Targets, AActor, {
		Ptr->SetActorEnableCollision(bToggle ? !Ptr->GetActorEnableCollision() : bCollision);
	})
}

void FWAActorTags::OnBeginPlay()
{
	Add.Remove(NAME_None);
	Remove.Remove(NAME_None);
}

void FWAActorTags::RunAction()
{
	FOR_EACH_SOFT_PTR(Targets, AActor, {
		for (const FName& Tag : Add) { Ptr->Tags.AddUnique(Tag); }
		Ptr->Tags.RemoveAll([this](const FName& Tag)
		{
			return Remove.Contains(Tag);
		});
	})
}

void FWASMActorMaterials::RunAction()
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

void FWASMActorPrimitiveData::RunAction()
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
