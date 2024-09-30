// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "HeartBeatGameWidget.generated.h"

UCLASS()
class FAF_REV_API UHeartBeatGameWidget : public UUserWidget
{
	GENERATED_BODY()

	friend class UHeartBeatGame;

public:

private:

	void AddWidget(const FString& Sequence);
};
