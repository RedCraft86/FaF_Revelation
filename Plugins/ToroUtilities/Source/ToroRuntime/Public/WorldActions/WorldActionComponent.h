// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "WorldActionBase.h"
#include "Components/ActorComponent.h"
#include "StructUtils/InstancedStruct.h"
#include "WorldActionComponent.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class TORORUNTIME_API UWorldActionComponent final : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UWorldActionComponent();
	
	UPROPERTY(EditAnywhere, Category = Settings, NoClear, meta = (ExcludeBaseStruct, HideViewOptions, ShowTreeView))
		TArray<TInstancedStruct<FWorldActionBase>> Actions;

	UFUNCTION(BlueprintCallable, Category = WorldEvents)
		void RunEvents();
	
protected:

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
#if WITH_EDITOR
	virtual void PostLoad() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
