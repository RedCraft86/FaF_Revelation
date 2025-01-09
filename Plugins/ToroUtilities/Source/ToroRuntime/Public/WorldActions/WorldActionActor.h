// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "WorldActionComponent.h"
#include "WorldActionActor.generated.h"

UCLASS(meta = (HiddenCategories = "Collision, Actor"))
class TORORUNTIME_API AWorldActionActor final : public AActor
{
	GENERATED_BODY()

public:

	AWorldActionActor();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleDefaultsOnly, Category = Subobjects)
		TObjectPtr<UBillboardComponent> DefaultIconBillboard;
#endif

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<UWorldActionComponent> WorldActions;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "WorldEvents")
		void RunEvents() const;
};
