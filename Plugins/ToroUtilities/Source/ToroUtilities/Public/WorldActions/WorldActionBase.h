// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "StructUtils/InstancedStruct.h"
#include "WorldActionBase.generated.h"

#define FOR_EACH_ACTION(ActionsArr, Context, ...) \
	for (TInstancedStruct<FWorldActionBase>& Action : ActionsArr) \
	{ \
		if (FWorldActionBase* Ptr = Action.GetMutablePtr()) \
		{ \
			Ptr->SetWorldContext(Context); \
			__VA_ARGS__ \
		} \
}

#define FOR_EACH_ACTION_CONST(ActionsArr, ...) \
	for (const TInstancedStruct<FWorldActionBase>& Action : ActionsArr) \
	{ \
		if (const FWorldActionBase* Ptr = Action.GetPtr()) \
		{ \
			__VA_ARGS__ \
		} \
	}

#define FOR_EACH_ACTION_PTR(ActionPtrsArr, Context, ...) \
	for (FWorldActionBase* Ptr : ActionPtrsArr) \
	{ \
		if (Ptr) \
		{ \
			Ptr->SetWorldContext(Context); \
			__VA_ARGS__ \
		} \
	}

#define FOR_EACH_SOFT_PTR(Property, Type, ...) \
	for (const TSoftObjectPtr<Type>& SoftPtr : Property) \
	{ \
		if (Type* Ptr = SoftPtr.LoadSynchronous()) \
		{ \
			__VA_ARGS__ \
		} \
	}

USTRUCT(BlueprintInternalUseOnly)
struct TOROUTILITIES_API FWorldActionBase
{
	GENERATED_BODY()

	FWorldActionBase() {}
	virtual ~FWorldActionBase() = default;

	virtual void RunAction() {}
	virtual void OnBeginPlay() {}
	virtual void OnConstruction() {}
	virtual void OnTick(const float DeltaTime) {}
	virtual bool ShouldTick() const { return bShouldTick; }

	void SetWorldContext(const UObject* Context) { if (Context) WorldContext = Context; }
	const UObject* GetWorldContext() const { return WorldContext.IsValid() ? WorldContext.Get() : nullptr; }

protected:

	UPROPERTY() bool bShouldTick = false;
	UPROPERTY(Transient) TWeakObjectPtr<const UObject> WorldContext;
};