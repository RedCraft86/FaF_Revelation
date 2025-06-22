// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroUtilities.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "NarrativeWidget.generated.h"

UCLASS(Abstract)
class FAFREVGAME_API UNarrativeWidget final : public UToroWidgetBase
{
	GENERATED_BODY()

public:

	UNarrativeWidget(const FObjectInitializer& ObjectInitializer);

private: // TODO

	virtual bool CanCreateWidget() const override { return UToroUtilSettings::Get()->IsOnGameplayMap(this); }
};
