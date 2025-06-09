// Copyright (C) RedCraft86. All Rights Reserved.

#include "Procedural/ToroProcGenBase.h"
#include "Algo/RandomShuffle.h"

AToroProcGenBase::AToroProcGenBase(): bRealtimeConstruction(true)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);
	
#if WITH_EDITORONLY_DATA
	SceneRoot->bVisualizeComponent = true;
	bRunConstructionScriptOnDrag = true;
#endif
}

void AToroProcGenBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (bRealtimeConstruction) Construct();
}

TArray<int32> AToroProcGenBase::LoopMeshArray(const TArray<FTransformMeshData>& Sample, const EGeneratorLoopMode Mode, const uint8 Amount)
{
	if (Sample.IsEmpty() || Amount == 0) 
		return {};

	TArray<int32> FilteredIndices;
	for (int32 i = 0; i < Sample.Num(); ++i)
	{
		if (Sample[i].IsValidData())
		{
			FilteredIndices.Add(i);
		}
	}

	if (FilteredIndices.IsEmpty()) 
		return {};

	TArray<int32> RetVal;
	RetVal.Reserve(Amount);
	for (int32 i = 0; i < Amount; ++i)
	{
		RetVal.Add(FilteredIndices[i % FilteredIndices.Num()]);
	}

	switch (Mode)
	{
	case EGeneratorLoopMode::Reverse:
		Algo::Reverse(RetVal);
		break;

	case EGeneratorLoopMode::Random:
		Algo::RandomShuffle(RetVal);
		break;

	default:
		break;
	}

	return RetVal;

}
