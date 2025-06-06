// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GlobalEventPayload.generated.h"

UCLASS(Blueprintable, BlueprintType)
class TOROUTILITIES_API UGlobalEventPayload : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category = EventPayload)
	TObjectPtr<UObject> EventInstigator;

	UPROPERTY(BlueprintReadOnly, Category = EventPayload, meta = (ExposeOnSpawn = true))
	TMap<FName, FString> Strings;

	UPROPERTY(BlueprintReadOnly, Category = EventPayload, meta = (ExposeOnSpawn = true))
	TMap<FName, float> Floats;
};