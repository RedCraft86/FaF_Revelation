// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "GeneralInterface.generated.h"

UINTERFACE()
class UToroGeneralInterface : public UInterface
{
	GENERATED_BODY()
};

class TORORUNTIME_API IToroGeneralInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = ExitInterface)
		void Exit();
	virtual void Exit_Implementation() {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = ExitInterface)
		void Return();
	virtual void Return_Implementation() {}

	static void Exit(UObject* Target)
	{
		if (IsValid(Target) && Target->Implements<UToroGeneralInterface>())
		{
			IToroGeneralInterface::Execute_Exit(Target);
		}
	}

	static void Return(UObject* Target)
	{
		if (IsValid(Target) && Target->Implements<UToroGeneralInterface>())
		{
			IToroGeneralInterface::Execute_Return(Target);
		}
	}
};