// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "NarrativeComponent.h"
#include "GameNarrative.generated.h"

UCLASS()
class FAFREVGAME_API UGameNarrative final : public UNarrativeComponent
{
	GENERATED_BODY()

public:

	UGameNarrative() {}

	static UGameNarrative* Get(const UObject* ContextObject);
	UFUNCTION(BlueprintPure, Category = Game, meta = (WorldContext = "ContextObject"))
		static UGameNarrative* GetNarrative(const UObject* ContextObject) { return Get(ContextObject); }
};
