// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "StructUtils/InstancedStruct.h"
#include "WorldActionBase.generated.h"

#define FOR_EACH_ACTION(Property, ...) \
	for (FWorldActionBase* ActionPtr : ActionPtrs) \
	{ \
		if (ActionPtr) \
			__VA_ARGS__ \
	}

#define FOR_EACH_SOFT_PTR(Property, Type, ...) \
	for (const TSoftObjectPtr<Type>& SoftPtr : Property) \
	{ \
		if (Type* Ptr = SoftPtr.LoadSynchronous()) \
			__VA_ARGS__ \
	}

UENUM(BlueprintType)
enum class EWorldActionType : uint8
{
	Set,
	Toggle
};

USTRUCT(BlueprintInternalUseOnly)
struct TOROUTILITIES_API FWorldActionBase
{
	GENERATED_BODY()

	FWorldActionBase(): bCanEverTick(false) {}
	virtual ~FWorldActionBase() = default;

	UPROPERTY() bool bCanEverTick;

	virtual void RunEvent(const UObject* WorldContext) {}
	virtual void OnBeginPlay(const UObject* WorldContext) {}
	virtual void OnTick(const UObject* WorldContext, const float DeltaTime) {}
	virtual void OnPostEditChange(const UObject* WorldContext) {}
	
protected:

	static inline uint8 LatentID = 0;
	static FLatentActionInfo GetLatentInfo()
	{
		FLatentActionInfo Info;
		Info.Linkage = 0;
		Info.UUID = LatentID++;
		Info.CallbackTarget = nullptr;
		Info.ExecutionFunction = NAME_None;
		return Info;
	}
};