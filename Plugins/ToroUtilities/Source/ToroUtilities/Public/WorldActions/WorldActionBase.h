// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "StructUtils/InstancedStruct.h"
#include "WorldActionBase.generated.h"

USTRUCT(BlueprintInternalUseOnly)
struct TOROUTILITIES_API FWorldActionBase
{
	GENERATED_BODY()

	FWorldActionBase() {}
	virtual ~FWorldActionBase() = default;
	UPROPERTY() bool bShouldTick = false;

	virtual void RunAction() {}
	virtual void OnBeginPlay() {}
	virtual void OnTick(const float DeltaTime) {}
	virtual void OnPostEditChange() {}

	void SetWorldContext(const UObject* Context) { if (Context) WorldContext = Context; }
	const UObject* GetWorldContext() const { return WorldContext.IsValid() ? WorldContext.Get() : nullptr; }

protected:

	UPROPERTY(Transient) TWeakObjectPtr<const UObject> WorldContext;
};