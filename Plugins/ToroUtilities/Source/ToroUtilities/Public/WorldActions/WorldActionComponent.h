// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "WorldActionBase.h"
#include "Components/ActorComponent.h"
#include "WorldActionComponent.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class TOROUTILITIES_API UWorldActionComponent final : public UActorComponent
{
	GENERATED_BODY()

public:

	UWorldActionComponent();

	UFUNCTION(BlueprintCallable, Category = WorldEvents)
		void SetActions(TArray<TInstancedStruct<FWorldActionBase>>& InActions);

	UFUNCTION(BlueprintCallable, Category = WorldEvents)
		void RunEvents();

protected:

	TArray<FWorldActionBase*> ActionPtrs;

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
#if WITH_EDITOR
	virtual void PostLoad() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
