// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ExecPinEnums.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"
#include "ToroCameraManager.generated.h"

UCLASS()
class TOROUTILITIES_API AToroCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:

	AToroCameraManager();

	UFUNCTION(BlueprintCallable, Category = Game, meta = (WorldContext = "ContextObject", DynamicOutputParam = "OutObject", DeterminesOutput = "Class", ExpandEnumAsExecs = "ReturnValue", AutoCreateRefTerm = "Class", CompactNodeTitle = "Get Player Camera Manager"))
	static EToroValidPins GetToroPlayerCameraManager(AToroCameraManager*& OutObject, const UObject* ContextObject, const TSubclassOf<AToroCameraManager>& Class, const int32 PlayerIndex = 0);

	template <typename T = AToroCameraManager>
	static T* Get(const UObject* ContextObject, const int32 PlayerIndex = 0)
	{
		return Cast<T>(UGameplayStatics::GetPlayerCameraManager(ContextObject, PlayerIndex));
	}

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};
