// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "WorldEventComponent.h"
#include "WorldEventActor.generated.h"

UCLASS(meta = (HiddenCategories = "Collision, Actor"))
class TOROGAMEPLAY_API AWorldEventActor final : public AActor
{
	GENERATED_BODY()

public:

	AWorldEventActor();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Subobjects")
		TObjectPtr<USceneComponent> SceneRoot;

#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleDefaultsOnly, Category = "Subobjects")
		TObjectPtr<UBillboardComponent> DefaultIconBillboard;
#endif

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Subobjects")
		TObjectPtr<UWorldEventComponent> WorldEvents;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "WorldEvents")
		void RunEvents() const;
};
