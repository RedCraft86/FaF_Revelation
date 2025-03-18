// Copyright (C) RedCraft86. All Rights Reserved.

#include "Inventory/InventoryItemActor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/InventoryItemData.h"

AInventoryItemActor::AInventoryItemActor() : Amount(0)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	InstancedStaticMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>("InstancedStaticMesh");
	InstancedStaticMesh->SetCollisionProfileName(TEXT("NoCollision"));
	InstancedStaticMesh->SetupAttachment(GetRootComponent());

	bSingleUse = false;
	Interaction.Label = NSLOCTEXT("Toro", "Take", "Take");
}

void AInventoryItemActor::FillAmount(const uint8 InAmount)
{
	if (InAmount != 0)
	{
		Amount += InAmount;
		SetEnabled(true);
	}
}

FInteractionInfo AInventoryItemActor::GetInteractionInfo_Implementation(const FHitResult& HitResult)
{
	if (ItemData && Amount > 0)
	{
		FFormatNamedArguments Args;
		Args.Add(TEXT("Amount"), Amount);
		Args.Add(TEXT("Item"), ItemData->DisplayName);
		Interaction.Label = UInventoryItemData::InjectMetadataToText(
			FText::Format(Interaction.Label, Args), Metadata);
		return Interaction;
	}

	return FInteractionInfo::GetEmpty();
}

void AInventoryItemActor::OnBeginInteract_Implementation(AToroPlayerCharacter* Player, const FHitResult& HitResult)
{
	Super::OnBeginInteract_Implementation(Player, HitResult);
	if (ItemData && Amount > 0)
	{
		if (UInventoryComponent* Inventory = UInventoryComponent::Get(this))
		{
			Amount = Inventory->AddItem(ItemData, Amount, Metadata);
		}
	}

	if (Amount == 0)
	{
		SetEnabled(false);
	}
}

void AInventoryItemActor::SetEnabled(const bool bInEnabled)
{
	Super::SetEnabled(Amount > 0 && bInEnabled);
}

void AInventoryItemActor::OnConstruction(const FTransform& Transform)
{
	AToroVolume::OnConstruction(Transform);
	if (Interaction.Label.IsEmptyOrWhitespace())
	{
		Interaction.Label = NSLOCTEXT("Toro", "Take", "Take");
	}

	InstancedStaticMesh->ClearInstances();
	if (MeshInstances.IsEmpty())
		MeshInstances.Add(FTransform::Identity);

	if (ItemData)
	{
		const FTransformMeshData MeshData = ItemData->GetMeshData(Metadata);
		if (MeshData.IsValidData() && Amount > 0)
		{
			InstancedStaticMesh->AddInstances(MeshInstances, false);
			UPrimitiveDataLibrary::SetStaticMeshProperties(InstancedStaticMesh, MeshData);
		}
	}
}

#if WITH_EDITOR
bool AInventoryItemActor::CanEditChange(const FProperty* InProperty) const
{
	return Super::CanEditChange(InProperty) && InProperty->GetFName() != GET_MEMBER_NAME_CHECKED(ThisClass, bSingleUse);
}
#endif