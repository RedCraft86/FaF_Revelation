﻿// Copyright (C) RedCraft86. All Rights Reserved.

#include "DataTypes/PrimitiveData.h"
#include "Helpers/CollisionHelpers.h"

void FStaticMeshData::FillMaterials()
{
	if (Mesh.IsNull())
	{
		Materials.Empty();
	}
	else if (UStaticMesh* SM = Mesh.LoadSynchronous())
	{
		if (const TArray<FStaticMaterial>& Mats = SM->GetStaticMaterials(); Materials.Num() < Mats.Num())
		{
			for (int32 i = 0; i < Mats.Num(); i++)
			{
				if (!Materials.IsValidIndex(i))
				{
					Materials.Add(Mats[i].MaterialInterface);
				}
				else if (Materials[i].IsNull())
				{
					Materials[i] = Mats[i].MaterialInterface;
				}
			}
		}
	}
}

void FStaticMeshData::FromStaticMeshComponent(const UStaticMeshComponent* InComponent)
{
	if (!IsValid(InComponent)) return;
	Mesh = InComponent->GetStaticMesh();
	bCastShadows = InComponent->CastShadow;
	TArray<UMaterialInterface*> Mats = InComponent->GetMaterials();
	Materials.Empty(Mats.Num());
	for (UMaterialInterface* Material : Mats)
	{
		Materials.Add(TObjectPtr<UMaterialInterface>(Material));
	}

	FillMaterials();
}

void FStaticMeshData::ToStaticMeshComponent(UStaticMeshComponent* InComponent) const
{
	if (!IsValid(InComponent)) return;
	FStaticMeshData TempCopy = *this;
	TempCopy.FillMaterials();
	InComponent->SetStaticMesh(TempCopy.Mesh.LoadSynchronous());
	InComponent->SetCastShadow(TempCopy.bCastShadows);
	for (int i = 0; i < TempCopy.Materials.Num(); i++)
	{
		InComponent->SetMaterial(i, TempCopy.Materials[i].LoadSynchronous());
	}
}

void FPrimitiveCollision::SetProfileName(const FName& InName)
{
	const UCollisionProfile* Profile = UCollisionProfile::Get();
	if (FCollisionResponseTemplate ProfileData; Profile->GetProfileTemplate(InName, ProfileData))
	{
		CollisionProfileName = ProfileData.Name;
		CollisionEnabled = ProfileData.CollisionEnabled;
		ObjectType = ProfileData.ObjectType;
		CollisionHelpers::ForEachChannel([this, ProfileData](const ECollisionChannel Channel)
		{
			Responses.Add(Channel, ProfileData.ResponseToChannels.GetResponse(Channel));
		}, false);
	}
	else
	{
		CollisionProfileName = UCollisionProfile::CustomCollisionProfileName;
	}

	FillResponsesMap();
}

void FPrimitiveCollision::SetCollisionEnabled(const ECollisionEnabled::Type InCollisionEnabled)
{
	CollisionProfileName = UCollisionProfile::CustomCollisionProfileName;
	CollisionEnabled = InCollisionEnabled;
}

void FPrimitiveCollision::SetObjectType(const ECollisionChannel InObjectType)
{
	CollisionProfileName = UCollisionProfile::CustomCollisionProfileName;
	ObjectType = InObjectType;
}

void FPrimitiveCollision::SetAllResponses(const ECollisionResponse InResponse)
{
	CollisionProfileName = UCollisionProfile::CustomCollisionProfileName;
	CollisionHelpers::ForEachChannel([this, InResponse](const ECollisionChannel Channel)
	{
		Responses.Add(Channel, InResponse);
	}, false);
}

void FPrimitiveCollision::SetResponse(const ECollisionChannel InChannel, const ECollisionResponse InResponse)
{
	CollisionProfileName = UCollisionProfile::CustomCollisionProfileName;
	Responses.Add(InChannel, InResponse);
}

ECollisionResponse FPrimitiveCollision::GetResponse(const ECollisionChannel InChannel) const
{
	return Responses.Contains(InChannel) ? (ECollisionResponse)Responses.FindRef(InChannel) : ECR_Ignore;
}

void FPrimitiveCollision::FillResponsesMap(const ECollisionResponse InResponses)
{
	CollisionHelpers::ForEachChannel([this, InResponses](const ECollisionChannel Channel)
	{
		if (!Responses.Contains(Channel))
		{
			Responses.Add(Channel, CollisionHelpers::IsHiddenChannel(Channel) ? ECR_Ignore : InResponses);
		}
	}, false);
}

void FPrimitiveCollision::FromPrimitiveComponent(const UPrimitiveComponent* InComponent)
{
	if (!IsValid(InComponent)) return;
	if (CollisionHelpers::IsValidProfile(InComponent->GetCollisionProfileName()))
	{
		SetProfileName(InComponent->GetCollisionProfileName());
#if !WITH_EDITOR
	}
	else
	{
#else
		CollisionProfileName = UCollisionProfile::CustomCollisionProfileName;
#endif
		CollisionEnabled = InComponent->GetCollisionEnabled();
		ObjectType = InComponent->GetCollisionObjectType();
		CollisionHelpers::ForEachChannel([this, InComponent](const ECollisionChannel Channel)
		{
			Responses.Add(Channel, InComponent->GetCollisionResponseToChannel(Channel));
		}, false);
	}
}

void FPrimitiveCollision::ToPrimitiveComponent(UPrimitiveComponent* InComponent) const
{
	if (!IsValid(InComponent)) return;
	if (CollisionHelpers::IsValidProfile(CollisionProfileName))
	{
		InComponent->SetCollisionProfileName(CollisionProfileName);
	}
#if !WITH_EDITOR
	else
#endif
	{
		InComponent->SetCollisionEnabled(CollisionEnabled);
		InComponent->SetCollisionObjectType(ObjectType);
		CollisionHelpers::ForEachChannel([this, InComponent](const ECollisionChannel Channel)
		{
			InComponent->SetCollisionResponseToChannel(Channel, Responses.FindRef(Channel));
		}, false);
	}
}

void UPrimitiveDataLibrary::FillMaterials(FStaticMeshData& Target)
{
	Target.FillMaterials();
}

bool UPrimitiveDataLibrary::IsValid_MeshData(const FStaticMeshData& Target)
{
	return Target.IsValidData();
}

TArray<FVector> UPrimitiveDataLibrary::GetBoundingBoxVertices(const AActor* Target, const bool bOnlyCollidingComponents,
	const bool bIncludeFromChildActors, FVector& Origin, FVector& BoxExtent)
{
	if (!IsValid(Target)) return {};
	Target->GetActorBounds(bOnlyCollidingComponents, Origin, BoxExtent, bIncludeFromChildActors);
	
	TArray<FVector> Result;
	Result.AddUnique(Origin + BoxExtent);
	Result.AddUnique(Origin - BoxExtent);
	Result.AddUnique(FVector(Origin.X - BoxExtent.X, Origin.Y + BoxExtent.Y, Origin.Z + BoxExtent.Z));
	Result.AddUnique(FVector(Origin.X + BoxExtent.X, Origin.Y - BoxExtent.Y, Origin.Z + BoxExtent.Z));
	Result.AddUnique(FVector(Origin.X + BoxExtent.X, Origin.Y + BoxExtent.Y, Origin.Z - BoxExtent.Z));
	Result.AddUnique(FVector(Origin.X - BoxExtent.X, Origin.Y - BoxExtent.Y, Origin.Z + BoxExtent.Z));
	Result.AddUnique(FVector(Origin.X + BoxExtent.X, Origin.Y - BoxExtent.Y, Origin.Z - BoxExtent.Z));
	Result.AddUnique(FVector(Origin.X - BoxExtent.X, Origin.Y + BoxExtent.Y, Origin.Z - BoxExtent.Z));

	return Result;
}

FPrimitiveCollision UPrimitiveDataLibrary::Make_PrimitiveCollision(const FName InProfileName,
	const TEnumAsByte<ECollisionEnabled::Type> InCollisionEnabled, const TEnumAsByte<ECollisionChannel> InObjectType,
	const TMap<TEnumAsByte<ECollisionChannel>, TEnumAsByte<ECollisionResponse>>& InResponses)
{
	if (CollisionHelpers::IsValidProfile(InProfileName))
	{
		return {InProfileName};
	}
	
	return {InCollisionEnabled, InObjectType, InResponses};
}

void UPrimitiveDataLibrary::Break_PrimitiveCollision(const FPrimitiveCollision& Target, FName& ProfileName,
	TEnumAsByte<ECollisionEnabled::Type>& CollisionEnabled, TEnumAsByte<ECollisionChannel>& ObjectType,
	TMap<TEnumAsByte<ECollisionChannel>, TEnumAsByte<ECollisionResponse>>& Responses)
{
	ProfileName = Target.GetProfileName();
	CollisionEnabled = Target.GetCollisionEnabled();
	ObjectType = Target.GetObjectType();
	Responses = Target.GetResponsesMap();
}

void UPrimitiveDataLibrary::SetCollisionProfileName(FPrimitiveCollision& Target, const FName InProfileName)
{
	Target.SetProfileName(InProfileName);
}

FName UPrimitiveDataLibrary::GetCollisionProfileName(const FPrimitiveCollision& Target)
{
	return Target.GetProfileName();
}

void UPrimitiveDataLibrary::SetAllCollisionResponses(FPrimitiveCollision& Target, const ECollisionResponse InResponse)
{
	Target.SetAllResponses(InResponse);
}

void UPrimitiveDataLibrary::SetCollisionResponse(FPrimitiveCollision& Target, const ECollisionChannel InChannel, const ECollisionResponse InResponse)
{
	Target.SetResponse(InChannel, InResponse);
}

ECollisionResponse UPrimitiveDataLibrary::GetCollisionResponse(const FPrimitiveCollision& Target, const ECollisionChannel InChannel)
{
	return Target.GetResponse(InChannel);
}

void UPrimitiveDataLibrary::SetCollisionEnabled(FPrimitiveCollision& Target, const ECollisionEnabled::Type InEnabled)
{
	Target.SetCollisionEnabled(InEnabled);
}

ECollisionEnabled::Type UPrimitiveDataLibrary::GetCollisionEnabled(const FPrimitiveCollision& Target)
{
	return Target.GetCollisionEnabled();
}

void UPrimitiveDataLibrary::SetCollisionObjectType(FPrimitiveCollision& Target, const ECollisionChannel InObjectType)
{
	Target.SetObjectType(InObjectType);
}

ECollisionChannel UPrimitiveDataLibrary::GetCollisionObjectType(const FPrimitiveCollision& Target)
{
	return Target.GetObjectType();
}

FStaticMeshData UPrimitiveDataLibrary::GetStaticMeshData(const UStaticMeshComponent* Target)
{
	FStaticMeshData Result;
	Result.FromStaticMeshComponent(Target);
	return Result;
}

void UPrimitiveDataLibrary::SetStaticMeshData(UStaticMeshComponent* Target, const FStaticMeshData& Data)
{
	Data.ToStaticMeshComponent(Target);
}

FPrimitiveCollision UPrimitiveDataLibrary::GetPrimitiveCollision(const UPrimitiveComponent* Target)
{
	FPrimitiveCollision Result;
	Result.FromPrimitiveComponent(Target);
	return Result;
}

void UPrimitiveDataLibrary::SetPrimitiveCollision(UPrimitiveComponent* Target, const FPrimitiveCollision& Data)
{
	Data.ToPrimitiveComponent(Target);
}