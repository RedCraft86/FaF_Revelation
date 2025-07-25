﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "DataTypes/PrimitiveData.h"
#include "ToroProcGenBase.generated.h"

UENUM(BlueprintType)
enum class EGeneratorLoopMode : uint8
{
	Forward,
	Reverse,
	Random
};

UCLASS(Abstract)
class TOROUTILITIES_API AToroProcGenBase : public AActor
{
	GENERATED_BODY()

public:

	AToroProcGenBase();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
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

	UPROPERTY(EditAnywhere, Category = Settings, AdvancedDisplay)
		bool bRealtimeConstruction;
	
	UFUNCTION(CallInEditor, Category = Tools)
		virtual void Construct() { EventConstruct(); }
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Construct")
		void EventConstruct();

	virtual void OnConstruction(const FTransform& Transform) override;

public:
	
	static TArray<int32> LoopMeshArray(const TArray<FTransformMeshData>& Sample, const EGeneratorLoopMode Mode, const uint8 Amount);
};
