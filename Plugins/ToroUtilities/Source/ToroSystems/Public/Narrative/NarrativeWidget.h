// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroUtilities.h"
#include "UserWidgets/ToroWidgetBase.h"
#include "NarrativeWidget.generated.h"

UCLASS(Abstract)
class TOROSYSTEMS_API UQuestWidget final : public UToroWidgetBase
{
	GENERATED_BODY()

public:

	UQuestWidget(const FObjectInitializer& ObjectInitializer);

private:

	virtual bool CanCreateWidget() const override { return UToroUtilSettings::Get()->IsOnGameplayMap(this); }
};

UCLASS(Abstract)
class TOROSYSTEMS_API UDialogueWidget final : public UToroWidgetBase
{
	GENERATED_BODY()

public:

	UDialogueWidget(const FObjectInitializer& ObjectInitializer);

private:

	virtual bool CanCreateWidget() const override { return UToroUtilSettings::Get()->IsOnGameplayMap(this); }
};
