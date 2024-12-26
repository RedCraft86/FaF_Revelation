﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "InstancedStaticMeshActor.generated.h"

class UInstancedStaticMeshComponent;

UCLASS(MinimalAPI, ConversionRoot, ComponentWrapperClass, HideCategories = (Input), ShowCategories = ("Input|MouseInput", "Input|TouchInput"), meta = (ChildCanTick))
class AInstancedStaticMeshActor : public AActor
{
	GENERATED_BODY()

public:

	AInstancedStaticMeshActor(const FObjectInitializer& ObjectInitializer);

#if WITH_EDITORONLY_DATA
	UPROPERTY(Category = StaticMesh, EditAnywhere, meta = (MakeEditWidget, DisplayPriority = 1))
		TArray<FTransform> Instances = {FTransform::Identity};;
#endif
	
	/** This static mesh should replicate movement. Automatically sets the RemoteRole and bReplicateMovement flags. Meant to be edited on placed actors (those other two properties are not) */
	UPROPERTY(Category = Actor, EditAnywhere, AdvancedDisplay)
		bool bStaticMeshReplicateMovement;

	/** Set which replication mode to use for this static mesh instance if it both replicates and simulates physics. */
	UPROPERTY(Category = Actor, EditAnywhere, AdvancedDisplay)
		EPhysicsReplicationMode StaticMeshPhysicsReplicationMode;

	/** Function to change mobility type */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Mobility)
		TOROCORE_API void SetMobility(const EComponentMobility::Type InMobility) const;

	/** Returns StaticMeshComponent subobject **/
	UInstancedStaticMeshComponent* GetStaticMeshComponent() const { return StaticMeshComponent; }

#if WITH_EDITOR
	TOROCORE_API virtual void PostLoad() override;	
	TOROCORE_API virtual void CheckForErrors() override;
	TOROCORE_API virtual bool GetReferencedContentObjects( TArray<UObject*>& Objects ) const override;
#endif
	TOROCORE_API virtual void Serialize(FArchive& Ar) override;
	virtual ENavDataGatheringMode GetGeometryGatheringMode() const { return ENavDataGatheringMode::Default; }

	/** Name of the StaticMeshComponent.  Use this name if you want to use a different class (with ObjectInitializer.SetDefaultSubobjectClass). */
	static TOROCORE_API FName StaticMeshComponentName;

protected:
	
#if WITH_EDITOR
	TOROCORE_API virtual void LoadedFromAnotherClass(const FName& OldClassName) override;
	TOROCORE_API virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
	TOROCORE_API virtual void OnConstruction(const FTransform& Transform) override;
#endif
	TOROCORE_API virtual FString GetDetailedInfoInternal() const override;
	TOROCORE_API virtual void BeginPlay() override;
	
private:
	
	UPROPERTY(Category = StaticMeshActor, VisibleAnywhere, BlueprintReadOnly, meta = (ExposeFunctionCategories = "Mesh,Rendering,Physics,Components|StaticMesh", AllowPrivateAccess = true))
		TObjectPtr<UInstancedStaticMeshComponent> StaticMeshComponent;
};

UCLASS()
class AHierarchicalInstancedStaticMeshActor : public AInstancedStaticMeshActor
{
	GENERATED_BODY()

public:

	AHierarchicalInstancedStaticMeshActor(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer.SetDefaultSubobjectClass<UHierarchicalInstancedStaticMeshComponent>(StaticMeshComponentName))
	{
		bCanBeInCluster = GetClass() == AHierarchicalInstancedStaticMeshActor::StaticClass();
	}
};