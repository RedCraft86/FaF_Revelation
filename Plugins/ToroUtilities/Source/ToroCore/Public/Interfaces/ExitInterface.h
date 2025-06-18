#pragma once

#include "UObject/Interface.h"
#include "ExitInterface.generated.h"

UINTERFACE()
class UExitInterface : public UInterface
{
	GENERATED_BODY()
};

class TOROCORE_API IExitInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = ToroInterfaces)
		void Exit();
	virtual void Exit_Implementation() {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = ToroInterfaces)
		void ReturnToWidget(UUserWidget* FromWidget);
	virtual void ReturnToWidget_Implementation(UUserWidget* FromWidget) {}

	static void Exit(UObject* Target)
	{
		if (IsValid(Target) && Target->Implements<UExitInterface>())
		{
			IExitInterface::Execute_Exit(Target);
		}
	}

	static void ReturnToWidget(UObject* Target, UUserWidget* FromWidget)
	{
		if (IsValid(Target) && Target->Implements<UExitInterface>())
		{
			IExitInterface::Execute_ReturnToWidget(Target, FromWidget);
		}
	}
};
