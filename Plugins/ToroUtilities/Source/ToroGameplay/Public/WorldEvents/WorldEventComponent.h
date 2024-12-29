// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "WorldEventBase.h"
#include "Components/ActorComponent.h"
#include "StructUtils/InstancedStruct.h"
#include "WorldEventComponent.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class TOROGAMEPLAY_API UWorldEventComponent final : public UActorComponent
{
	GENERATED_BODY()

public:

	UWorldEventComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (BaseStruct = "/Script/ToroGameplay.WorldEventBase", ExcludeBaseStruct, ShowTreeView))
		TArray<FInstancedStruct> Events;

	UFUNCTION(BlueprintCallable, Category = "WorldEvents")
		void SetupEvents();

	UFUNCTION(BlueprintCallable, Category = "WorldEvents")
		void RunEvents();

protected:
	
	TSet<FWorldEventBase*> EventPtrs;

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
#if WITH_EDITOR
	virtual void PostLoad() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
