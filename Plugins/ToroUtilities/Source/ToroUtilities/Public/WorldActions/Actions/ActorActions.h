// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "WorldActions/WorldActionBase.h"
#include "ActorActions.generated.h"

USTRUCT(BlueprintInternalUseOnly, DisplayName = "Actor", meta = (Hidden))
struct TOROUTILITIES_API FWAActorActions : public FWorldActionBase
{ GENERATED_BODY() };

USTRUCT(BlueprintType, DisplayName = "Visibility")
struct TOROUTILITIES_API FWAActorVisibility final : public FWAActorActions
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = Visibility)
		bool bToggle;

	UPROPERTY(EditAnywhere, Category = Visibility, meta = (EditCondition = "!bToggle", EditConditionHides))
		bool bVisible;

	UPROPERTY(EditAnywhere, Category = Visibility)
		TSet<TSoftObjectPtr<AActor>> Targets;
	
	FWAActorVisibility(): bToggle(false), bVisible(true) {}

protected:

	virtual void RunAction() override;
};

USTRUCT(BlueprintType, DisplayName = "Collision")
struct TOROUTILITIES_API FWAActorCollision final : public FWAActorActions
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = Visibility)
		bool bToggle;

	UPROPERTY(EditAnywhere, Category = Visibility, meta = (EditCondition = "!bToggle", EditConditionHides))
		bool bCollision;

	UPROPERTY(EditAnywhere, Category = Visibility)
		TSet<TSoftObjectPtr<AActor>> Targets;
	
	FWAActorCollision(): bToggle(false), bCollision(true) {}

protected:

	virtual void RunAction() override;
};

USTRUCT(BlueprintType, DisplayName = "Tags")
struct TOROUTILITIES_API FWAActorTags final : public FWAActorActions
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = Tags)
		TSet<FName> Add;

	UPROPERTY(EditAnywhere, Category = Tags)
		TSet<FName> Remove;

	UPROPERTY(EditAnywhere, Category = Tags)
	TSet<TSoftObjectPtr<AActor>> Targets;
	
	FWAActorTags() {}

protected:

	virtual void OnBeginPlay() override;
	virtual void RunAction() override;
};

USTRUCT(BlueprintInternalUseOnly, DisplayName = "Static Mesh", meta = (Hidden))
struct TOROUTILITIES_API FWASMActorActions : public FWAActorActions
{ GENERATED_BODY() };

USTRUCT(BlueprintType, DisplayName = "Materials")
struct TOROUTILITIES_API FWASMActorMaterials final : public FWASMActorActions
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Materials)
		TMap<uint8, TSoftObjectPtr<UMaterialInterface>> Materials;

	UPROPERTY(EditAnywhere, Category = Materials)
		TSet<TSoftObjectPtr<AStaticMeshActor>> Targets;

	FWASMActorMaterials() {}

protected:

	virtual void RunAction() override;
};

USTRUCT(BlueprintType, DisplayName = "PrimitiveData")
struct TOROUTILITIES_API FWASMActorPrimitiveData final : public FWASMActorActions
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = PrimitiveData)
		bool bSerializable;

	UPROPERTY(EditAnywhere, Category = PrimitiveData)
		TMap<uint8, float> Data;

	UPROPERTY(EditAnywhere, Category = PrimitiveData)
		TSet<TSoftObjectPtr<AStaticMeshActor>> Targets;

	FWASMActorPrimitiveData(): bSerializable(false) {}

protected:

	virtual void RunAction() override;
};