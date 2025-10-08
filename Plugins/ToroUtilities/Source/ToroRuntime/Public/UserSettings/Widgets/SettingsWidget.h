// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserInterface/ToroManagedWidget.h"
#include "UserSettings/ToroUserSettings.h"
#include "SettingsWidget.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API USettingsWidget : public UToroManagedWidget
{
	GENERATED_BODY()

public:

	USettingsWidget(const FObjectInitializer& ObjectInitializer);

	void ShowWidget(UUserWidget* InParent);

protected:

	TObjectPtr<UUserWidget> ParentUI;
	TObjectPtr<UToroUserSettings> Settings;

	// TODO impl

	virtual void PopWidget() override;
	virtual void PushWidget() override;
	virtual void InitWidget(APlayerController* Controller) override;
};
