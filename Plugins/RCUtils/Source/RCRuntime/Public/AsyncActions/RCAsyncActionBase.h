// Copyright (C) Tayzar (RedCraft86). All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "RCAsyncActionBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRCAsyncActionDelegate);

UCLASS(Abstract)
class RCRUNTIME_API URCAsyncActionBase : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	
	void SetWorldContext(UObject* InObject) { WorldContext = InObject; }
	FORCEINLINE UObject* GetWorldContext() const { return WorldContext; }
	FORCEINLINE bool IsValidWorldContext() const
	{
		if (IsValid(WorldContext) && GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull))
			return true;

		FFrame::KismetExecutionMessage(TEXT("Invalid World Context Object!"), ELogVerbosity::Warning);
		return false;
	}

	virtual UWorld* GetWorld() const override
	{
		if (!GEngine) return nullptr; // How can this happen?
#if WITH_EDITOR
		if (FApp::IsGame())
#endif
		{
			if (IsValidWorldContext())
			{
				return GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
			}
			
			FFrame::KismetExecutionMessage(TEXT("Cannot get world from context using last resort solution"),
					ELogVerbosity::Warning);
				
			return GEngine->GetCurrentPlayWorld();
		}
#if WITH_EDITOR
		return GEngine->GetCurrentPlayWorld();
#endif
	}

protected:

	UPROPERTY(Transient)
		UObject* WorldContext = nullptr;
};
