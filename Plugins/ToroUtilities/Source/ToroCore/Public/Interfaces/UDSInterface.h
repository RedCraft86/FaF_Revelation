// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UObject/Object.h"
#include "UDSInterface.generated.h"

UINTERFACE()
class UUDSInterface : public UInterface
{
	GENERATED_BODY()
};

class TOROCORE_API IUDSInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = UDSInterface)
	void SetSettings(const FUDSSettings& InSettings);

	static void SetSettings(UObject* Target, const FUDSSettings& InSettings)
	{
		if (IsValid(Target) && Target->Implements<UUDSInterface>())
		{
			IUDSInterface::Execute_SetSettings(Target, InSettings);
		}
	}
};
