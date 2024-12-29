// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "WorldEvents/WorldEventBase.h"
#include "ActorEvents.generated.h"

USTRUCT(BlueprintInternalUseOnly, DisplayName = "Actors", meta = (Hidden))
struct TOROGAMEPLAY_API FWEActorsBase : public FWorldEventBase
{ GENERATED_BODY() };

USTRUCT(BlueprintType, DisplayName = "Visibility")
struct TOROGAMEPLAY_API FWEActorVisibility final : public FWEActorsBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "ActorVisibility")
		EWorldEventChangeType ChangeType;

	UPROPERTY(EditAnywhere, Category = "ActorVisibility", meta = (EditCondition = "ChangeType == EWorldEventChangeType::Set", EditConditionHides))
		bool bVisible;

	UPROPERTY(EditAnywhere, Category = "ActorVisibility")
		TSet<TSoftObjectPtr<AActor>> Targets;

	FWEActorVisibility() : ChangeType(EWorldEventChangeType::Set), bVisible(true), Targets({}) {}

protected:
	
	virtual void RunEvent(const UObject* WorldContext) override;
};

USTRUCT(BlueprintType, DisplayName = "Collision")
struct TOROGAMEPLAY_API FWEActorCollision final : public FWEActorsBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "ActorCollision")
		EWorldEventChangeType ChangeType;

	UPROPERTY(EditAnywhere, Category = "ActorCollision", meta = (EditCondition = "ChangeType == EWorldEventChangeType::Set", EditConditionHides))
		bool bCollision;

	UPROPERTY(EditAnywhere, Category = "ActorCollision")
		TSet<TSoftObjectPtr<AActor>> Targets;

	FWEActorCollision() : ChangeType(EWorldEventChangeType::Set), bCollision(true), Targets({}) {}

protected:
	
	virtual void RunEvent(const UObject* WorldContext) override;
};

USTRUCT(BlueprintType, DisplayName = "Tags")
struct TOROGAMEPLAY_API FWEActorTags final : public FWEActorsBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "ActorTags")
		bool bRemoval;

	UPROPERTY(EditAnywhere, Category = "ActorTags")
		TSet<FName> Tags;

	UPROPERTY(EditAnywhere, Category = "ActorTags")
		TSet<TSoftObjectPtr<AActor>> Targets;

	FWEActorTags() : bRemoval(false), Tags({}), Targets({}) {}

protected:

	virtual void OnBeginPlay(const UObject* WorldContext) override;
	virtual void RunEvent(const UObject* WorldContext) override;
};

USTRUCT(BlueprintType, DisplayName = "Enabled")
struct TOROGAMEPLAY_API FWEActorEnabled final : public FWEActorsBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "ActorEnabled")
		EWorldEventChangeType ChangeType;

	UPROPERTY(EditAnywhere, Category = "ActorEnabled", meta = (EditCondition = "ChangeType == EWorldEventChangeType::Set", EditConditionHides))
		bool bEnabled;

	UPROPERTY(EditAnywhere, Category = "ActorEnabled", meta = (AllowedClasses = "/Script/ToroCore.ToroActor,/Script/ToroCore.ToroActor"))
		TSet<TSoftObjectPtr<AActor>> Targets;

	FWEActorEnabled() : ChangeType(EWorldEventChangeType::Set), bEnabled(true), Targets({}) {}

protected:
	
	virtual void RunEvent(const UObject* WorldContext) override;
};

USTRUCT(BlueprintInternalUseOnly, DisplayName = "Static Mesh", meta = (Hidden))
struct TOROGAMEPLAY_API FWEStaticMeshActorBase : public FWEActorsBase
{ GENERATED_BODY() };

USTRUCT(BlueprintType, DisplayName = "Material")
struct TOROGAMEPLAY_API FWEStaticMeshMaterial final : public FWEStaticMeshActorBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "MeshMaterial")
		TMap<uint8, TSoftObjectPtr<UMaterialInterface>> Materials;

	UPROPERTY(EditAnywhere, Category = "MeshMaterial")
		TSet<TSoftObjectPtr<AStaticMeshActor>> Targets;

	FWEStaticMeshMaterial() : Materials({}), Targets({}) {}

protected:
	
	virtual void RunEvent(const UObject* WorldContext) override;
};

USTRUCT(BlueprintType, DisplayName = "Primitive Data")
struct TOROGAMEPLAY_API FWEStaticMeshPrimitiveData final : public FWEStaticMeshActorBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "MeshPrimitiveData")
		TMap<uint8, float> Data;

	UPROPERTY(EditAnywhere, Category = "MeshPrimitiveData")
		TSet<TSoftObjectPtr<AStaticMeshActor>> Targets;

	FWEStaticMeshPrimitiveData() : Data({}), Targets({}) {}

protected:
	
	virtual void RunEvent(const UObject* WorldContext) override;
};