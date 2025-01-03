// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ElectricActorBase.h"
#include "StructUtils/InstancedStruct.h"
#include "WorldActions/WorldActionBase.h"
#include "ElectricWorldActions.generated.h"

UCLASS(NotBlueprintable)
class TORORUNTIME_API AElectricWorldActions final : public AElectricActorBase
{
	GENERATED_BODY()
	
public:

	AElectricWorldActions();

	UPROPERTY(EditAnywhere, Category = "Settings", NoClear, meta = (ExcludeBaseStruct, HideViewOptions, ShowTreeView))
		TArray<TInstancedStruct<FWorldActionBase>> Activate;

	UPROPERTY(EditAnywhere, Category = "Settings", NoClear, meta = (ExcludeBaseStruct, HideViewOptions, ShowTreeView))
		TArray<TInstancedStruct<FWorldActionBase>> Deactivate;

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnStateChanged(const bool bState) override;
#if WITH_EDITOR
	virtual void PostLoad() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
