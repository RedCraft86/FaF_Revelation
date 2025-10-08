// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserSettings/Widgets/SettingsWidget.h"
#include "Interfaces/ExitInterface.h"

USettingsWidget::USettingsWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAutoPush = false;
}

void USettingsWidget::ShowWidget(UUserWidget* InParent)
{
	ParentUI = InParent;
	PushWidget();
}

void USettingsWidget::PopWidget()
{
	Settings->ApplySettings(true);
	IExitInterface::ReturnToWidget(ParentUI, this);
	ParentUI = nullptr;
	Super::PopWidget();
}

void USettingsWidget::PushWidget()
{
	Super::PushWidget();
}

void USettingsWidget::InitWidget(APlayerController* Controller)
{
	Super::InitWidget(Controller);
	Settings = UToroUserSettings::Get();
}
