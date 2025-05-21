// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "ISMActor.generated.h"

class UInstancedStaticMeshComponent;

UCLASS(ConversionRoot, ComponentWrapperClass, HideCategories = (Input, Physics), ShowCategories = ("Input|MouseInput", "Input|TouchInput"), meta = (ChildCanTick))
class TOROCORE_API AInstancedStaticMeshActor : public AActor
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
		void SetMobility(const EComponentMobility::Type InMobility) const;

	/** Returns StaticMeshComponent subobject **/
	UInstancedStaticMeshComponent* GetStaticMeshComponent() const { return StaticMeshComponent; }

#if WITH_EDITOR
	virtual void PostLoad() override;	
	virtual void CheckForErrors() override;
	virtual bool GetReferencedContentObjects( TArray<UObject*>& Objects ) const override;
#endif
	virtual void Serialize(FArchive& Ar) override;
	virtual ENavDataGatheringMode GetGeometryGatheringMode() const { return ENavDataGatheringMode::Default; }

	/** Name of the StaticMeshComponent.  Use this name if you want to use a different class (with ObjectInitializer.SetDefaultSubobjectClass). */
	static FName StaticMeshComponentName;

protected:
	
#if WITH_EDITOR
	virtual void LoadedFromAnotherClass(const FName& OldClassName) override;
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
	virtual FString GetDetailedInfoInternal() const override;
	virtual void BeginPlay() override;
	
private:
	
	UPROPERTY(Category = StaticMeshActor, VisibleAnywhere, BlueprintReadOnly, meta = (ExposeFunctionCategories = "Mesh,Rendering,Physics,Components|StaticMesh", AllowPrivateAccess = true))
		TObjectPtr<UInstancedStaticMeshComponent> StaticMeshComponent;
};

UCLASS()
class TOROCORE_API AHierarchicalInstancedStaticMeshActor : public AInstancedStaticMeshActor
{
	GENERATED_BODY()

public:

	AHierarchicalInstancedStaticMeshActor(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer.SetDefaultSubobjectClass<UHierarchicalInstancedStaticMeshComponent>(StaticMeshComponentName))
	{
		bCanBeInCluster = GetClass() == AHierarchicalInstancedStaticMeshActor::StaticClass();
	}
};