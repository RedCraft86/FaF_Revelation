// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ExecPinEnums.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include "ToroWidgetManager.generated.h"

/* Player State is repurposed as a Widget Manager */

UCLASS()
class TORORUNTIME_API AToroWidgetManager : public APlayerState
{
	GENERATED_BODY()

public:

	AToroWidgetManager();

	UFUNCTION(BlueprintCallable, Category = Game, meta = (WorldContext = "WorldContextObject", DynamicOutputParam = "OutObject", DeterminesOutput = "Class", ExpandEnumAsExecs = "ReturnValue", AutoCreateRefTerm = "Class", CompactNodeTitle = "Get Widget Manager"))
		static EToroValidPins GetToroWidgetManager(AToroWidgetManager*& OutObject, const UObject* WorldContextObject, const TSubclassOf<AToroWidgetManager>& Class, const int32 PlayerIndex = 0);

	template <typename T = AToroWidgetManager>
	static T* Get(const UObject* WorldContextObject, const int32 PlayerIndex = 0)
	{
		return Cast<T>(UGameplayStatics::GetPlayerState(WorldContextObject, PlayerIndex));
	}
	
protected:

	virtual void BeginPlay() override;
};
