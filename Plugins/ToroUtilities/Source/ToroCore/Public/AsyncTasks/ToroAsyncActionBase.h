﻿// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "ToroAsyncActionBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FToroAsyncActionDelegate);

#define BEGIN_CREATE_TASK ThisClass* Task = NewObject<ThisClass>();
#define FINISH_CREATE_TASK return Task;

#define BEGIN_RUNNING if (IsRunningChecked()) return;
#define FINISH_RUNNING bTaskRunning = false;

UCLASS(Abstract)
class TOROCORE_API UToroAsyncActionBase : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:

	UToroAsyncActionBase(): bTaskRunning(false), WorldContext(nullptr) {}
	UObject* GetWorldContext() const { return WorldContext.IsValid() ? WorldContext.Get() : nullptr; }
	void SetWorldContext(UObject* InContext) { if (InContext) WorldContext = InContext; }
	virtual UWorld* GetWorld() const override;

protected:
	
	bool IsRunningChecked();
	UPROPERTY() bool bTaskRunning;
	UPROPERTY(Transient) TWeakObjectPtr<UObject> WorldContext;
};
