// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "WorldActions/WorldActionBase.h"
#include "ActorActions.generated.h"

USTRUCT(BlueprintInternalUseOnly, DisplayName = "Actor Actions", meta = (Hidden))
struct TOROUTILITIES_API FWTaskActorActions : public FWorldActionBase
{ GENERATED_BODY() };

USTRUCT(BlueprintType, DisplayName = "Visibility")
struct TOROUTILITIES_API FWTaskActorVisibility final : public FWTaskActorActions
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = Visibility)
		EWorldActionType ChangeType;

	UPROPERTY(EditAnywhere, Category = Visibility, meta = (EditCondition = "ChangeType == EWorldActionType::Set", EditConditionHides))
		bool bVisible;

	UPROPERTY(EditAnywhere, Category = Visibility)
		TSet<TSoftObjectPtr<AActor>> Targets;
	
	FWTaskActorVisibility(): ChangeType(EWorldActionType::Set), bVisible(true) {}

protected:

	virtual void RunEvent(const UObject* WorldContext) override;
};

USTRUCT(BlueprintType, DisplayName = "Collision")
struct TOROUTILITIES_API FWTaskActorCollision final : public FWTaskActorActions
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = Collision)
		EWorldActionType ChangeType;

	UPROPERTY(EditAnywhere, Category = Collision, meta = (EditCondition = "ChangeType == EWorldActionType::Set", EditConditionHides))
		bool bCollision;

	UPROPERTY(EditAnywhere, Category = Collision)
		TSet<TSoftObjectPtr<AActor>> Targets;

	FWTaskActorCollision(): ChangeType(EWorldActionType::Set), bCollision(true) {}

protected:

	virtual void RunEvent(const UObject* WorldContext) override;
};

USTRUCT(BlueprintType, DisplayName = "Tags")
struct TOROUTILITIES_API FWTaskActorTags final : public FWTaskActorActions
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Tags)
		bool bRemoval;

	UPROPERTY(EditAnywhere, Category = Tags)
		TSet<FName> Tags;

	UPROPERTY(EditAnywhere, Category = Tags)
		TSet<TSoftObjectPtr<AActor>> Targets;

	FWTaskActorTags(): bRemoval(false) {}

protected:

	virtual void OnBeginPlay(const UObject* WorldContext) override;
	virtual void RunEvent(const UObject* WorldContext) override;
};

USTRUCT(BlueprintType, DisplayName = "Enabled")
struct TOROUTILITIES_API FWTaskActorEnabled final : public FWTaskActorActions
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Enabled)
		EWorldActionType ChangeType;

	UPROPERTY(EditAnywhere, Category = Enabled, meta = (EditCondition = "ChangeType == EWorldActionType::Set", EditConditionHides))
		bool bEnabled;

	UPROPERTY(EditAnywhere, Category = Enabled, meta = (AllowedClasses = "/Script/ToroCore.ToroActor,/Script/ToroCore.ToroVolume"))
		TSet<TSoftObjectPtr<AActor>> Targets;
	
	FWTaskActorEnabled(): ChangeType(EWorldActionType::Set), bEnabled(true) {}

protected:

	virtual void RunEvent(const UObject* WorldContext) override;
};

USTRUCT(BlueprintInternalUseOnly, DisplayName = "Static Mesh", meta = (Hidden))
struct TOROUTILITIES_API FWTaskSMActorActions : public FWTaskActorActions
{ GENERATED_BODY() };

USTRUCT(BlueprintType, DisplayName = "Override Materials")
struct TOROUTILITIES_API FWTaskSMMaterial final : public FWTaskSMActorActions
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Materials)
		TMap<uint8, TSoftObjectPtr<UMaterialInterface>> Materials;

	UPROPERTY(EditAnywhere, Category = Materials)
		TSet<TSoftObjectPtr<AStaticMeshActor>> Targets;
	
	FWTaskSMMaterial() {}

protected:

	virtual void RunEvent(const UObject* WorldContext) override;
};

USTRUCT(BlueprintType, DisplayName = "Primitive Data")
struct TOROUTILITIES_API FWTaskSMPrimitiveData final : public FWTaskSMActorActions
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = PrimitiveData)
		bool bSerializable;

	UPROPERTY(EditAnywhere, Category = PrimitiveData)
		TMap<uint8, float> Data;

	UPROPERTY(EditAnywhere, Category = PrimitiveData)
		TSet<TSoftObjectPtr<AStaticMeshActor>> Targets;

	FWTaskSMPrimitiveData(): bSerializable(false) {}

protected:

	virtual void RunEvent(const UObject* WorldContext) override;
};