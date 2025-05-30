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

	UPROPERTY(EditAnywhere, Category = Settings, NoClear, meta = (ExcludeBaseStruct, HideViewOptions, ShowTreeView))
		TArray<TInstancedStruct<FWorldActionBase>> NativeActions;

	UFUNCTION(BlueprintCallable, Category = WorldActions)
		void SetActions(UPARAM(ref) TArray<TInstancedStruct<FWorldActionBase>>& InActions);

	UFUNCTION(BlueprintCallable, Category = WorldActions)
		void RunActions();

private:

	TArray<FWorldActionBase*> ActionPtrs;
	void ForEachAction(const TFunction<void(FWorldActionBase*)>& Func);

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc) override;
#if WITH_EDITOR
	virtual void PostLoad() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
