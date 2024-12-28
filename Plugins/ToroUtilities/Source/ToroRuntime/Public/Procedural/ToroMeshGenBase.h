// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "DataTypes/PrimitiveData.h"
#include "Components/SplineMeshComponent.h"
#include "ToroMeshGenBase.generated.h"

UENUM(BlueprintType)
enum class EGeneratorLoopMode : uint8
{
	Forward,
	Reverse,
	Random
};

USTRUCT(BlueprintType)
struct TORORUNTIME_API FTransformMeshData : public FStaticMeshProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshProperties", meta = (DisplayPriority = 1))
		FTransform Transform;

	FTransformMeshData() : Transform(FTransform::Identity) {}
	FStaticMeshProperties operator*() const { return {Mesh, Materials, bCastShadows}; }
};

USTRUCT(BlueprintType)
struct TORORUNTIME_API FSplineMeshData : public FStaticMeshProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshProperties", meta = (DisplayPriority = -1))
		TEnumAsByte<ESplineMeshAxis::Type> MeshAxis;

	FSplineMeshData() : MeshAxis(ESplineMeshAxis::X) {}
	FStaticMeshProperties operator*() const { return {Mesh, Materials, bCastShadows}; }
};

UCLASS(Abstract)
class TORORUNTIME_API AToroMeshGenBase : public AActor
{
	GENERATED_BODY()

public:

	AToroMeshGenBase();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Subobjects")
		TObjectPtr<USceneComponent> SceneRoot;
	
	template<typename T>
	T* AddGenericComponent()
	{
		if (T* GeneratedComp = NewObject<T>(this))
		{
			GeneratedComp->CreationMethod = EComponentCreationMethod::UserConstructionScript;
			GeneratedComp->OnComponentCreated();
			GeneratedComp->RegisterComponent();
			GeneratedComp->SetMobility(EComponentMobility::Movable);
			GeneratedComp->AttachToComponent(GetRootComponent(),
				FAttachmentTransformRules::KeepRelativeTransform);
		
			return GeneratedComp;
		}

		return nullptr;
	}
	
protected:

	UPROPERTY(EditAnywhere, Category = "Settings", AdvancedDisplay)
		bool bRealtimeConstruction;
	
	UFUNCTION(CallInEditor, Category = "Tools")
		virtual void Construct() { EventConstruct(); }
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Construct")
		void EventConstruct();

	virtual void OnConstruction(const FTransform& Transform) override;

public:
	
	static TArray<int32> LoopMeshArray(const TArray<FTransformMeshData>& Sample, const EGeneratorLoopMode Mode, const uint8 Amount);
};
