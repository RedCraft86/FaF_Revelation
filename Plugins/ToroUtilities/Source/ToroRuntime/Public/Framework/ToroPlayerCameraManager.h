// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ExecPinEnums.h"
#include "Camera/PlayerCameraManager.h"
#include "ToroPlayerCameraManager.generated.h"

UCLASS()
class TORORUNTIME_API AToroPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:

	AToroPlayerCameraManager();

	UFUNCTION(BlueprintCallable, Category = "Game", meta = (WorldContext = "WorldContextObject", DynamicOutputParam = "OutObject", DeterminesOutput = "Class", ExpandEnumAsExecs = "ReturnValue", AutoCreateRefTerm = "Class", CompactNodeTitle = "Get Player Camera Manager"))
		static EToroValidPins GetToroPlayerCameraManager(AToroPlayerCameraManager*& OutObject, const UObject* WorldContextObject, const int32 PlayerIndex = 0, const TSubclassOf<AToroPlayerCameraManager>& Class = nullptr);

	template <typename T = AToroPlayerCameraManager>
	static T* Get(const UObject* WorldContextObject, const int32 PlayerIndex = 0);

protected:
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};
