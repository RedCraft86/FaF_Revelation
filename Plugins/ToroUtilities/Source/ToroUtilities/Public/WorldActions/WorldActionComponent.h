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

	UPROPERTY(EditAnywhere, Category = Settings)
		bool bManualEdFunction;

	UFUNCTION(BlueprintCallable, Category = WorldActions)
		void SetActions(UPARAM(ref) TArray<TInstancedStruct<FWorldActionBase>>& InActions);

	UFUNCTION(BlueprintCallable, Category = WorldActions)
		void AppendActions(UPARAM(ref) TArray<TInstancedStruct<FWorldActionBase>>& InActions);

	UFUNCTION(BlueprintCallable, Category = WorldActions)
		void RunActions();

	void UpdateEdFunctions();

private:

	TArray<FWorldActionBase*> ActionPtrs;

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc) override;
#if WITH_EDITOR
	virtual void PostLoad() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
