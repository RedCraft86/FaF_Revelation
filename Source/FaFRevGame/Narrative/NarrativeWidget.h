// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroSettings.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "NarrativeWidget.generated.h"

UCLASS(Abstract)
class FAFREVGAME_API UNarrativeWidget final : public UToroWidgetBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = NarrativeWidget)
		void SetQuestsHidden(const bool bInHidden) {}

	UFUNCTION(BlueprintPure, Category = NarrativeWidget)
		bool AreQuestsHidden() const { return bHideQuests; }

private:

	UPROPERTY() bool bHideQuests;

	virtual bool CanCreateWidget() const override { return UToroSettings::Get()->IsOnGameplayMap(this); }
};
