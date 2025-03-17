// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "ToroInterface.generated.h"

UINTERFACE()
class UToroInterface : public UInterface
{
	GENERATED_BODY()
};

class TOROCORE_API IToroInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = GeneralInterface)
		void Return();
	virtual void Return_Implementation() {}

	static void Return(UObject* Target)
	{
		if (IsValid(Target) && Target->Implements<UToroInterface>())
		{
			IToroInterface::Execute_Return(Target);
		}
	}
};