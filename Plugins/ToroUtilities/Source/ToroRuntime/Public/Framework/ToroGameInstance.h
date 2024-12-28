// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ExecPinEnums.h"
#include "Engine/GameInstance.h"
#include "ToroGameInstance.generated.h"

UCLASS()
class TORORUNTIME_API UToroGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UToroGameInstance();

	UFUNCTION(BlueprintCallable, Category = "Game", meta = (WorldContext = "WorldContextObject", DynamicOutputParam = "OutObject", DeterminesOutput = "Class", ExpandEnumAsExecs = "ReturnValue", AutoCreateRefTerm = "Class", CompactNodeTitle = "Get Game Instance"))
		static EToroValidPins GetToroGameInstance(UToroGameInstance*& OutObject, const UObject* WorldContextObject, const TSubclassOf<UToroGameInstance>& Class = nullptr);

	template <typename T = UToroGameInstance>
	static T* Get(const UObject* WorldContextObject);

	UFUNCTION(BlueprintImplementableEvent)
		void OnWorldBeginPlay(UWorld* InWorld);
	virtual void OnWorldBeginPlay_Implementation(UWorld* InWorld);

	UFUNCTION(BlueprintImplementableEvent)
		void OnWorldTick(const float DeltaSeconds);
	virtual void OnWorldTick_Implementation(const float DeltaSeconds) {}

	UFUNCTION(BlueprintCallable, Category = "GameInstance")
		void SetUnlitViewmode(const bool bUnlit);

protected:

	UPROPERTY() int32 CachedVMI;
	
	virtual void Init() override;
};
